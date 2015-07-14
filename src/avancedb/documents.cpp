#include "documents.h"

Documents::Documents(database_ptr db) : db_(db) {
    
}

documents_ptr Documents::Create(database_ptr db) {
    return documents_ptr(new Documents(db));
}