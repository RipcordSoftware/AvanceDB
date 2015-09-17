/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015 Ripcord Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
        collections_(GetCollectionCount()), docsMtx_(collections_),
        allDocsCacheDocs_(boost::make_shared<document_array>()),
        allDocsCacheUpdateSequence_(0) {
   
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

document_ptr Documents::GetDocument(const char* id, bool throwOnFail) {
    auto coll = GetDocumentCollectionIndex(id);
    
    boost::lock_guard<DocumentsMutex> guard{docsMtx_[coll]};
    
    Document::Compare compare{id};
    auto doc = docs_[coll].find_fn(compare);
    
    if (!doc && throwOnFail) {
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
    
    ++updateSeq_;
    
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

document_ptr Documents::GetDesignDocument(const char* id, bool throwOnFail) {
    std::string designId = "_design/";
    designId += id;
    return GetDocument(designId.c_str(), throwOnFail);
}

document_ptr Documents::DeleteDesignDocument(const char* id, const char* rev) {
    std::string designId = "_design/";
    designId += id;
    return DeleteDocument(designId.c_str(), rev);
}

document_ptr Documents::SetDesignDocument(const char* id, script_object_ptr obj) {
    std::string designId = "_design/";
    designId += id;
    return SetDocument(designId.c_str(), obj);
}

document_array_ptr Documents::GetAllDocuments(sequence_type& updateSequence) {
    boost::unique_lock<decltype(allDocsCacheMtx_)> guard(allDocsCacheMtx_);
    updateSequence = allDocsCacheUpdateSequence_;
    
    if (allDocsCacheUpdateSequence_ == updateSeq_) {
        return allDocsCacheDocs_;
    } else {
        auto count = getCount();

        auto allDocs = boost::make_shared<document_array>();
        allDocs->reserve(count);
        
        std::vector<boost::unique_lock<DocumentsMutex>> locks{docsMtx_.begin(), docsMtx_.end()};
        
        updateSequence = updateSeq_;
        allDocsCacheUpdateSequence_ = updateSequence;

        for (unsigned i = 0; i < collections_; ++i) {
            auto oldSize = allDocs->size();
            allDocs->insert(allDocs->end(), docs_[i].cbegin(), docs_[i].cend());
            locks[i].unlock();
            
            if (i > 0) {
                std::inplace_merge(allDocs->begin(), allDocs->begin() + oldSize, allDocs->end(), Document::Less{});
            }
        }

        allDocsCacheDocs_ = allDocs;

        return allDocs;
    }
}

document_array_ptr Documents::GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& offset, collection::size_type& totalDocs, sequence_type& updateSequence) {       
    auto docs = GetAllDocuments(updateSequence);
    
    if (options.Descending()) {
        docs = boost::make_shared<document_array>(docs->begin(), docs->end());        
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
        docs = boost::make_shared<document_array>();
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

BulkDocumentsResults Documents::PostBulkDocuments(script_array_ptr docs, bool newEdits) {
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