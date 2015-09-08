#include "documents.h"

#include <algorithm>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>

#include "document.h"
#include "rest_exceptions.h"
#include "database.h"
#include "document_revision.h"
#include "config.h"
#include "uuid_helper.h"

Documents::Documents(database_ptr db) : db_(db), updateSeq_(0), localUpdateSeq_(0),
        collections_(GetCollectionCount()), docsMtx_(collections_) {
   
    for (unsigned i = 0; i < collections_; ++i) {
        docs_.emplace_back(64, 32 * 1024);
    }
}

documents_ptr Documents::Create(database_ptr db) {
    return documents_ptr(new Documents(db));
}

Documents::collection::size_type Documents::getCount() {
    collection::size_type size = 0;
    
    for (auto coll : docs_) {
        size += coll.size();
    }
    
    return size;
}

sequence_type Documents::getUpdateSequence() {
    return updateSeq_;
}

document_ptr Documents::GetDocument(const char* id) {
    auto coll = GetDocumentCollectionIndex(id);
    
    boost::lock_guard<DocumentsMutex> guard{docsMtx_[coll]};
    
    Document::Compare compare{id};
    auto doc = docs_[coll].find_fn(compare);
    
    if (!doc) {
        throw DocumentMissing();
    }
    
    return doc;
}

document_ptr Documents::DeleteDocument(const char* id, const char* rev) {
    auto coll = GetDocumentCollectionIndex(id);
    
    boost::lock_guard<DocumentsMutex> guard{docsMtx_[coll]};
    
    Document::Compare compare{id};
    auto doc = docs_[coll].find_fn(compare);
    
    if (!doc) {
        throw DocumentMissing();
    }
    
    DocumentRevision::Validate(rev, true);
    
    auto docRev = doc->getRev();
    if (std::strcmp(rev, docRev) != 0) {
        throw DocumentConflict();
    }
    
    docs_[coll].erase(doc);
    
    return doc;
}

document_ptr Documents::SetDocument(const char* id, script_object_ptr obj) {
    auto coll = GetDocumentCollectionIndex(id);
    
    boost::lock_guard<DocumentsMutex> guard{docsMtx_[coll]};
    
    Document::Compare compare{id};
    auto doc = docs_[coll].find_fn(compare);
    
    auto objRev = obj->getString("_rev", false);

    if (!!doc) {                     
        auto docRev = doc->getRev();
        
        if (objRev == nullptr || std::strcmp(objRev, docRev) != 0) {
            throw DocumentConflict();
        }
    } else if (objRev != nullptr) {
        DocumentRevision::Validate(objRev, true);
    }

    doc = Document::Create(id, obj, ++updateSeq_);

    docs_[coll].insert(doc);

    return doc;
}

document_array_ptr Documents::GetAllDocuments(sequence_type& updateSequence) {
    auto count = getCount();
    
    auto allDocs = boost::make_shared<document_array>();
    allDocs->reserve(count);
    
    for (unsigned i = 0; i < collections_; ++i) {
        boost::unique_lock<DocumentsMutex> guard{docsMtx_[i]};
        auto old_size = allDocs->size();
        allDocs->insert(allDocs->end(), docs_[i].cbegin(), docs_[i].cend());
        
        guard.unlock();
        std::inplace_merge(allDocs->begin(), allDocs->begin() + old_size, allDocs->end(), Document::Less{});
    }
    
    updateSequence = updateSeq_;
    
    return allDocs;
}

document_array_ptr Documents::GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& offset, collection::size_type& totalDocs, sequence_type& updateSequence) {       
    auto docs = GetAllDocuments(updateSequence);
    
    if (options.Descending()) {
        std::reverse(docs->begin(), docs->end());
    }

    collection::size_type startIndex = 0;
    collection::size_type endIndex = docs->size();
    collection::size_type indexSkip = options.Skip();
    collection::size_type indexLimit = options.Limit();

    if (options.HasKey()) {
        startIndex = FindDocument(*docs, options.Key(), options.Descending());
        if ((startIndex & findMissedFlag) == findMissedFlag) {
            startIndex = ~startIndex;
            endIndex = startIndex;
        } else {
            endIndex = startIndex + 1;
        }
    }
    else if (options.HasKeys()) {
        if (options.StartKey().size() > 0) {
            startIndex = FindDocument(*docs, options.StartKey(), options.Descending());
            if ((startIndex & findMissedFlag) == findMissedFlag) {
                startIndex = ~startIndex;
            }
        }

        if (options.EndKey().size() > 0) {
            endIndex = FindDocument(*docs, options.EndKey(), options.Descending());
            if ((endIndex & findMissedFlag) == findMissedFlag) {
                endIndex = ~endIndex;
            } else if (options.InclusiveEnd()) {
                endIndex++;
            }
        }
    }

    startIndex += indexSkip;

    offset = startIndex;
    totalDocs = docs->size();

    if (startIndex < docs->size() && startIndex < endIndex) {
        endIndex = std::min(startIndex + indexLimit, endIndex);
        endIndex = std::min(endIndex, docs->size());

        docs = boost::make_shared<document_array>(docs->cbegin() + startIndex, docs->cbegin() + endIndex);
    } else {
        docs->clear();
    }

    return docs;
}

document_array_ptr Documents::PostDocuments(const PostAllDocumentsOptions& options, Documents::collection::size_type& totalDocs, sequence_type& updateSequence) {
    auto docs = GetAllDocuments(updateSequence);
    
    const auto& keys = options.Keys();
    
    auto results = boost::make_shared<document_array>();
    results->reserve(keys.size());
    
    for (auto key : keys) {
        auto index = key.size() > 0 ? FindDocument(*docs, key, false) : findMissedFlag;
        if ((index & findMissedFlag) == 0) {
            auto doc = (*docs)[index];
            results->emplace_back(doc);
        } else {
            results->emplace_back(nullptr);
        }
    }
    
    totalDocs = docs->size();
    
    collection::size_type startIndex = options.Skip();
    collection::size_type endIndex = results->size();
    collection::size_type indexLimit = options.Limit();
    
    if (startIndex < endIndex) {
        endIndex = std::min(startIndex + indexLimit, endIndex);
        endIndex = std::min(endIndex, results->size());

        results = boost::make_shared<document_array>(results->cbegin() + startIndex, results->cbegin() + endIndex);
    } else {
        results->clear();
    }
    
    if (options.Descending()) {
        std::reverse(results->begin(), results->end());
    }
    
    return results;
}

Documents::BulkDocumentsResults Documents::PostBulkDocuments(script_array_ptr docs, bool newEdits) {
    BulkDocumentsResults results;
    UuidHelper::UuidGenerator gen;
    UuidHelper::UuidString newId;
    
    auto size = docs->getCount();    
    for (decltype(size) i = 0; i < size; ++i) {
        auto obj = docs->getObject(i);
        auto objRev = obj->getString("_rev", false);
        
        if (objRev) {
            DocumentRevision::Validate(objRev, true);
        }
    }
    
    for (decltype(size) i = 0; i < size; ++i) {
        auto obj = docs->getObject(i);
        
        auto id = obj->getString("_id", false);
        if (id == nullptr) {
            auto uuid = gen();
            UuidHelper::FormatUuid(uuid, newId);
            id = newId;
        }
        
        auto coll = GetDocumentCollectionIndex(id);
    
        boost::lock_guard<DocumentsMutex> guard{docsMtx_[coll]};
        
        Document::Compare compare{id};
        auto oldDoc = docs_[coll].find_fn(compare);
        
        auto objRev = obj->getString("_rev", false);
        
        const char* error = nullptr;
        const char* reason = nullptr;
        if (!!oldDoc && newEdits) {                     
            auto docRev = oldDoc->getRev();

            if (objRev == nullptr || std::strcmp(objRev, docRev) != 0) {
                error = "conflict";
                reason = "Document update conflict.";
            }
        }
        
        if (!error) {
            auto doc = Document::Create(id, obj, ++updateSeq_, newEdits);

            docs_[coll].insert(doc);

            auto newRev = doc->getRev();
            results.emplace_back(id, newRev); 
        } else {
            results.emplace_back(id, error, reason);
        }     
    }
    
    return std::move(results);
}

document_ptr Documents::GetLocalDocument(const char* id) {
    boost::lock_guard<boost::mutex> guard(localDocsMtx_);
    
    Document::Compare compare{id};
    auto doc = localDocs_.find_fn(compare);
    
    if (!doc) {
        throw DocumentMissing{};
    }
    
    return doc;
}

document_ptr Documents::SetLocalDocument(const char* id, script_object_ptr obj) {
    boost::lock_guard<boost::mutex> guard(localDocsMtx_);
    
    Document::Compare compare{id};
    auto doc = localDocs_.find_fn(compare);
    
    const char* objRev = obj->getString("_rev", false);

    if (!!doc) {
        auto docRev = doc->getRev();
        
        if (objRev == nullptr || std::strcmp(objRev, docRev) != 0) {
            throw DocumentConflict();
        }
    } else if (objRev != nullptr) {
        DocumentRevision::Validate(objRev, true);
    }
    
    doc = Document::Create(id, obj, ++localUpdateSeq_);

    localDocs_.insert(doc);

    return doc;
}

document_ptr Documents::DeleteLocalDocument(const char* id, const char* rev) {
    boost::lock_guard<boost::mutex> guard(localDocsMtx_);
    
    Document::Compare compare{id};
    auto doc = localDocs_.find_fn(compare);
    
    if (!doc) {
        throw DocumentMissing{};
    }
    
    DocumentRevision::Validate(rev, true);
    
    auto docRev = doc->getRev();
    if (std::strcmp(rev, docRev) != 0) {
        throw DocumentConflict{};
    }
    
    localDocs_.erase(doc);
    
    return doc;
}   

Documents::collection::size_type Documents::FindDocument(const document_array& docs, const std::string& id, bool descending) {
    const auto size = docs.size();
    
    if (size == 0) {
        return ~0;
    } else {
        collection::size_type min = 0;
        collection::size_type mid = 0;
        collection::size_type max = size - 1;

        while (min <= max) {
            mid = ((max - min) / 2) + min;

            auto diff = std::strcmp(id.c_str(), docs[mid]->getId());
            diff = descending ? -diff : diff;
            if (diff == 0) {
                return mid;
            } else if (diff < 0) {
                if (mid == 0) {
                    break;
                }
                
                max = mid - 1;
            } else {
                min = mid + 1;
            }
        }

        return ~min;
    }
}

unsigned Documents::GetCollectionCount() const {
    auto collections = Config::GetCPUCount() * 2;           
    return collections;
}

unsigned Documents::GetDocumentCollectionIndex(const char* id) const {
    auto hash = Document::getIdHash(id);
    auto index = hash % collections_;   
    return index;
}