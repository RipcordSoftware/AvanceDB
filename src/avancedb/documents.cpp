#include "documents.h"

#include <boost/bind.hpp>

#include "document.h"
#include "rest_exceptions.h"
#include "database.h"

Documents::Documents(database_ptr db) : db_(db), docs_(64, 32 * 1024),
    updateSeq_(0) {
    
}

documents_ptr Documents::Create(database_ptr db) {
    return documents_ptr(new Documents(db));
}

Documents::collection::size_type Documents::getCount() {
    return docs_.size();
}

sequence_type Documents::getUpdateSequence() {
    return updateSeq_;
}

document_ptr Documents::GetDocument(const char* id) {
    boost::lock_guard<boost::mutex> guard(docsMtx_);
    
    Document::Compare compare{id};
    auto doc = docs_.find_fn(compare);
    
    if (!doc) {
        throw DocumentMissing();
    }
    
    return doc;
}

document_ptr Documents::DeleteDocument(const char* id, const char* rev) {
    boost::lock_guard<boost::mutex> guard(docsMtx_);
    
    Document::Compare compare{id};
    auto doc = docs_.find_fn(compare);
    
    if (!doc) {
        throw DocumentMissing();
    }
    
    auto docRev = doc->getRev();
    if (std::strcmp(rev, docRev) != 0) {
        throw DocumentConflict();
    }
    
    docs_.erase(doc);
    
    return doc;
}

document_ptr Documents::SetDocument(const char* id, script_object_ptr obj) {
    boost::lock_guard<boost::mutex> guard(docsMtx_);
    
    Document::Compare compare{id};
    auto doc = docs_.find_fn(compare);

    if (!!doc) {
        if (obj->getType("_rev") != rs::scriptobject::ScriptObjectType::String) {
            throw DocumentConflict();
        }
        
        auto objRev = obj->getString("_rev");
        
        auto docRev = doc->getRev();
        
        if (std::strcmp(objRev, docRev) != 0) {
            throw DocumentConflict();
        }
    }

    doc = Document::Create(id, obj, ++updateSeq_);

    docs_.insert(doc);

    return doc;
}

document_array Documents::GetAllDocuments(sequence_type& updateSequence) {
    boost::lock_guard<boost::mutex> guard(docsMtx_);
    
    updateSequence = updateSeq_;
    
    return document_array{docs_.cbegin(), docs_.cend()};
}

document_array Documents::GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& offset, collection::size_type& totalDocs, sequence_type& updateSequence) {       
    auto docs = GetAllDocuments(updateSequence);
    
    if (options.Descending()) {
        std::reverse(docs.begin(), docs.end());
    }

    collection::size_type startIndex = 0;
    collection::size_type endIndex = docs.size();
    collection::size_type indexSkip = options.Skip();
    collection::size_type indexLimit = options.Limit();

    if (options.HasKey()) {
        startIndex = FindDocument(docs, options.Key(), options.Descending());
        if ((startIndex & findMissedFlag) == findMissedFlag) {
            startIndex = ~startIndex;
            endIndex = startIndex;
        } else {
            endIndex = startIndex + 1;
        }
    }
    else if (options.HasKeys()) {
        if (options.StartKey().size() > 0) {
            startIndex = FindDocument(docs, options.StartKey(), options.Descending());
            if ((startIndex & findMissedFlag) == findMissedFlag) {
                startIndex = ~startIndex;
            }
        }

        if (options.EndKey().size() > 0) {
            endIndex = FindDocument(docs, options.EndKey(), options.Descending());
            if ((endIndex & findMissedFlag) == findMissedFlag) {
                endIndex = ~endIndex;
            } else if (options.InclusiveEnd()) {
                endIndex++;
            }
        }
    }

    startIndex += indexSkip;

    offset = startIndex;
    totalDocs = docs.size();

    if (startIndex >= 0 && startIndex < docs.size() && startIndex < endIndex) {
        endIndex = std::min(startIndex + indexLimit, endIndex);
        endIndex = std::min(endIndex, docs.size());

        docs = document_array{docs.cbegin() + startIndex, docs.cbegin() + endIndex};
    } else {
        docs.clear();
    }

    return docs;
}

document_array Documents::PostDocuments(const PostAllDocumentsOptions& options, Documents::collection::size_type& totalDocs, sequence_type& updateSequence) {
    auto docs = GetAllDocuments(updateSequence);
    
    const auto& keys = options.Keys();
    
    document_array results;
    results.reserve(keys.size());
    
    for (auto key : keys) {
        auto index = FindDocument(docs, key, false);
        if ((index & findMissedFlag) == 0) {
            auto doc = docs[index];
            results.push_back(doc);
        } else {
            results.push_back(nullptr);
        }
    }
    
    totalDocs = docs.size();
    
    auto startIndex = options.Skip();
    auto endIndex = results.size();
    auto indexLimit = options.Limit();
    
    if (startIndex >= 0 && startIndex < endIndex) {
        endIndex = std::min(startIndex + indexLimit, endIndex);
        endIndex = std::min(endIndex, results.size());

        results = document_array{results.cbegin() + startIndex, results.cbegin() + endIndex};
    } else {
        results.clear();
    }
    
    if (options.Descending()) {
        std::reverse(results.begin(), results.end());
    }
    
    return results;
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