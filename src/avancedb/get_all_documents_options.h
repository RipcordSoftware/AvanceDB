#ifndef GET_ALL_DOCUMENTS_OPTIONS_H
#define	GET_ALL_DOCUMENTS_OPTIONS_H

#include "libhttpserver.h"

#include "types.h"

class GetAllDocumentsOptions final {
public:
    GetAllDocumentsOptions(const rs::httpserver::QueryString& qs);
    
    const std::string& Key();
    const std::string& StartKey();
    const std::string& StartKeyDocId();
    const std::string& EndKey();
    const std::string& EndKeyDocId();
    
    bool Conflicts();
    bool Descending();
    bool IncludeDocs();
    bool InclusiveEnd();
    bool UpdateSequence();
    
    int Skip();
    int Limit();
    
private:
    bool GetBoolean(const char* name, bool defaultValue);
    int GetInteger(const char* name, int defaultValue);
    
    const rs::httpserver::QueryString& qs_;
};

#endif	/* GET_ALL_DOCUMENTS_OPTIONS_H */

