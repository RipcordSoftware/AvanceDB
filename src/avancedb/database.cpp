/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2017 Ripcord Software
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

#include "database.h"

#include <boost/chrono.hpp>
#include <boost/make_shared.hpp>

#include "documents.h"

Database::Database(const char* name) : 
    name_(name), instanceStartTime_(Now()), docDelCount_(0) {
}

database_ptr Database::Create(const char* name) {
    auto ptr = boost::make_shared<database_ptr::element_type>(name);
    if (!!ptr) {
        ptr->docs_ = Documents::Create(ptr);
    }
    return ptr;
}

unsigned long Database::Now() {
    auto now = boost::chrono::system_clock::now().time_since_epoch();
    return boost::chrono::duration_cast<boost::chrono::microseconds>(now).count();
}

unsigned long Database::DocCount() { 
    return docs_->getCount(); 
}

unsigned long Database::DataSize() {
    return docs_->getDataSize();
}

unsigned long Database::DiskSize() { 
    return DataSize(); 
}

document_ptr Database::GetDocument(const char* id, bool throwOnFail) {
    return docs_->GetDocument(id, throwOnFail);
}

document_ptr Database::DeleteDocument(const char* id, const char* rev) {
    return docs_->DeleteDocument(id, rev);
}

document_ptr Database::SetDocument(const char* id, script_object_ptr obj) {
    return docs_->SetDocument(id, obj);
}

document_ptr Database::SetDocumentAttachment(const char* id, const char* rev, const char* name, const char* contentType, const std::vector<unsigned char>& attachment) {
    return docs_->SetDocumentAttachment(id, rev, name, contentType, attachment);
}

document_attachment_ptr Database::GetDocumentAttachment(const char* id, const char* name, bool includeBody) {
    return docs_->GetDocumentAttachment(id, name, includeBody);
}

document_ptr Database::DeleteDocumentAttachment(const char* id, const char* rev, const char* name) {
    return docs_->DeleteDocumentAttachment(id, rev, name);
}

document_ptr Database::GetDesignDocument(const char* id, bool throwOnFail) {
    return docs_->GetDesignDocument(id, throwOnFail);
}

document_ptr Database::DeleteDesignDocument(const char* id, const char* rev) {
    return docs_->DeleteDesignDocument(id, rev);
}

document_ptr Database::SetDesignDocument(const char* id, script_object_ptr obj) {
    return docs_->SetDesignDocument(id, obj);
}

document_array_ptr Database::GetDocuments(const GetAllDocumentsOptions& options, DocumentCollection::size_type& offset, DocumentCollection::size_type& totalDocs, sequence_type& updateSequence) {
    return docs_->GetDocuments(options, offset, totalDocs, updateSequence);
}

document_array_ptr Database::PostDocuments(const PostAllDocumentsOptions& options, DocumentCollection::size_type& totalDocs, sequence_type& updateSequence) {
    return docs_->PostDocuments(options, totalDocs, updateSequence);
}

document_ptr Database::GetLocalDocument(const char* id) {
    return docs_->GetLocalDocument(id);
}

document_ptr Database::DeleteLocalDocument(const char* id, const char* rev) {
    return docs_->DeleteLocalDocument(id, rev);
}

document_ptr Database::SetLocalDocument(const char* id, script_object_ptr obj) {
    return docs_->SetLocalDocument(id, obj);
}

BulkDocumentsResults Database::PostBulkDocuments(script_array_ptr docs, bool newEdits) {
    return docs_->PostBulkDocuments(docs, newEdits);
}

map_reduce_results_ptr Database::PostTempView(const GetViewOptions& options, rs::scriptobject::ScriptObjectPtr obj) {
    return docs_->PostTempView(options, obj);
}