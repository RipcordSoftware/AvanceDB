#ifndef GET_ALL_DOCUMENTS_OPTIONS_H
#define	GET_ALL_DOCUMENTS_OPTIONS_H

#include "libhttpserver.h"

#include "types.h"

class GetAllDocumentsOptions final {
public:
    GetAllDocumentsOptions(const rs::httpserver::QueryString& qs);
    
    const std::string& Key() const;
    const std::string& StartKey() const;
    const std::string& StartKeyDocId() const;
    const std::string& EndKey() const;
    const std::string& EndKeyDocId() const;
    
    bool Conflicts() const;
    bool Descending() const;
    bool IncludeDocs() const;
    bool InclusiveEnd() const;
    bool UpdateSequence() const;
    
    int Skip() const;
    int Limit() const;
    
private:
    bool GetBoolean(const char* name, bool defaultValue) const;
    int GetInteger(const char* name, int defaultValue) const;
    
    const rs::httpserver::QueryString& qs_;
};

#endif	/* GET_ALL_DOCUMENTS_OPTIONS_H */

