/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2016 Ripcord Software
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

#ifndef DATABASE_H
#define DATABASE_H

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/make_shared.hpp>

#include "documents.h"
#include "get_all_documents_options.h"
#include "post_all_documents_options.h"
#include "json_stream.h"
#include "get_view_options.h"

class Database final : public boost::enable_shared_from_this<Database>, private boost::noncopyable {
public:
        
    static database_ptr Create(const char* name);
    
    unsigned long CommitedUpdateSequence() { return docs_->getUpdateSequence(); }
    unsigned long UpdateSequence() { return docs_->getUpdateSequence(); }
    unsigned long PurgeSequence() { return 0; }
    unsigned long DataSize();
    unsigned long DiskSize();
    unsigned long DocCount();
    unsigned long DocDelCount() { return docDelCount_; }
    unsigned long InstanceStartTime() { return instanceStartTime_; }
    
    document_ptr GetDocument(const char* id, bool throwOnFail = true);
    document_ptr DeleteDocument(const char* id, const char* rev);
    document_ptr SetDocument(const char* id, script_object_ptr);
    
    document_ptr SetDocumentAttachment(const char* id, const char* rev, const char* name, const char* contentType, const std::vector<unsigned char>& attachment);
    document_attachment_ptr GetDocumentAttachment(const char* id, const char* name, bool includeBody);
    document_ptr DeleteDocumentAttachment(const char* id, const char* rev, const char* name);
    
    document_ptr GetDesignDocument(const char* id, bool throwOnFail = true);
    document_ptr DeleteDesignDocument(const char* id, const char* rev);
    document_ptr SetDesignDocument(const char* id, script_object_ptr);
    
    document_ptr GetLocalDocument(const char* id);
    document_ptr DeleteLocalDocument(const char* id, const char* rev);
    document_ptr SetLocalDocument(const char* id, script_object_ptr);
    
    document_array_ptr GetDocuments(const GetAllDocumentsOptions& options, DocumentCollection::size_type& offset, DocumentCollection::size_type& totalDocs, sequence_type& updateSequence);
    document_array_ptr PostDocuments(const PostAllDocumentsOptions& options, DocumentCollection::size_type& totalDocs, sequence_type& updateSequence);
    
    BulkDocumentsResults PostBulkDocuments(script_array_ptr docs, bool newEdits);
    
    map_reduce_results_ptr PostTempView(const GetViewOptions& options, rs::scriptobject::ScriptObjectPtr obj);
    
private:
    friend database_ptr boost::make_shared<database_ptr::element_type>(const char*&);

    Database(const char*);
    
    static unsigned long Now();
    
    const std::string name_;
    
    const unsigned long instanceStartTime_;
    boost::atomic<unsigned long> docDelCount_;
    
    documents_ptr docs_;
};

#endif	/* DATABASE_H */

