#include "documents.h"

#include "document.h"

Documents::Documents(database_ptr db) : db_(db) {
    
}

documents_ptr Documents::Create(database_ptr db) {
    return documents_ptr(new Documents(db));
}

document_ptr Documents::SetDocument(const char* id, script_object_ptr obj) {
    return Document::Create(id, obj);
}