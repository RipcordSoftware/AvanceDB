#include "database.h"

#include <boost/chrono.hpp>
#include <boost/make_shared.hpp>

#include "documents.h"

Database::Database(const char* name) : name_(name), instanceStartTime_(Now()),
    dataSize_(0), diskSize_(0), docDelCount_(0) {
}

database_ptr Database::Create(const char* name) {
    database_ptr ptr(new Database(name));
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

document_ptr Database::GetDocument(const char* id) {
    return docs_->GetDocument(id);
}

document_ptr Database::DeleteDocument(const char* id, const char* rev) {
    return docs_->DeleteDocument(id, rev);
}

document_ptr Database::SetDocument(const char* id, script_object_ptr obj) {
    return docs_->SetDocument(id, obj);
}

document_array_ptr Database::GetDocuments(const GetAllDocumentsOptions& options, Documents::collection::size_type& offset, Documents::collection::size_type& totalDocs, sequence_type& updateSequence) {
    return docs_->GetDocuments(options, offset, totalDocs, updateSequence);
}

document_array_ptr Database::PostDocuments(const PostAllDocumentsOptions& options, Documents::collection::size_type& totalDocs, sequence_type& updateSequence) {
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

Documents::BulkDocumentsResults Database::PostBulkDocuments(script_array_ptr docs, bool newEdits) {
    return docs_->PostBulkDocuments(docs, newEdits);
}