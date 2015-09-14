#ifndef BULK_DOCUMENTS_RESULT_H
#define BULK_DOCUMENTS_RESULT_H

#include <vector>

struct BulkDocumentsResult final {
    BulkDocumentsResult(const char* id, const char* rev) : 
        ok_(true), id_(id), rev_(rev) {}

    BulkDocumentsResult(const char* id, const char* error, const char* reason) : 
        ok_(false), id_(id), error_(error), reason_(reason) {}

    const bool ok_;
    const std::string id_;
    const std::string rev_;        
    const std::string error_;
    const std::string reason_;
};

using BulkDocumentsResults = std::vector<BulkDocumentsResult>;

#endif	/* BULK_DOCUMENTS_RESULT_H */
