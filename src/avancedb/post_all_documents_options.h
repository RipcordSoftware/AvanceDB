#ifndef POST_ALL_DOCUMENTS_OPTIONS_H
#define POST_ALL_DOCUMENTS_OPTIONS_H

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "libhttpserver.h"

#include "types.h"

class PostAllDocumentsOptions final {
public:
    using keys_array = std::vector<std::string>;
    
    PostAllDocumentsOptions(const rs::httpserver::QueryString& qs, script_array_ptr keysArray);
    
    const keys_array& Keys() const;
    
    bool Descending() const;
    bool IncludeDocs() const;
    bool InclusiveEnd() const;
    bool UpdateSequence() const;
    
    uint64_t Skip() const;
    uint64_t Limit() const;
    
private:
    bool GetBoolean(const char* name, bool defaultValue) const;
    uint64_t GetUnsigned(const char* name, uint64_t defaultValue) const;
            
    mutable keys_array keys_;
    
    mutable boost::optional<bool> conflicts_;
    mutable boost::optional<bool> descending_;
    mutable boost::optional<bool> includeDocs_;
    mutable boost::optional<bool> updateSequence_;
    
    mutable boost::optional<uint64_t> skip_;
    mutable boost::optional<uint64_t> limit_;
    
    const rs::httpserver::QueryString& qs_;
    const script_array_ptr keysArray_;
    
    const std::string _emptyString;
};

#endif	/* POST_ALL_DOCUMENTS_OPTIONS_H */

