#include "documents.h"

#include <boost/bind.hpp>

#include "document.h"
#include "rest_exceptions.h"

Documents::Documents(database_ptr db) : db_(db), docs_(64, 32 * 1024) {
    
}

documents_ptr Documents::Create(database_ptr db) {
    return documents_ptr(new Documents(db));
}

Documents::collection::size_type Documents::getCount() {
    return docs_.size();
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

document_ptr Documents::SetDocument(const char* id, script_object_ptr obj, sequence_type seqNum) {
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

    doc = Document::Create(id, obj, seqNum);

    docs_.insert(doc);

    return doc;
}

document_array Documents::GetAllDocuments() {
    boost::lock_guard<boost::mutex> guard(docsMtx_);
    
    return document_array{docs_.cbegin(), docs_.cend()};
}

document_array Documents::GetDocuments(const GetAllDocumentsOptions& options, collection::size_type& totalDocs) {       
    auto startkey = options.StartKey();
    if (startkey.size() == 0) {
        auto docs = GetAllDocuments();
        
        totalDocs = docs.size();

        if (options.Descending()) {
            std::reverse(docs.begin(), docs.end());
        }
        
        auto limit = options.Limit();
        if (limit < docs.size()) {
            docs = document_array{docs.cbegin(), docs.cbegin() + limit};
        }

        return docs;
    } else {
        totalDocs = getCount();
        return document_array{};
    }
}