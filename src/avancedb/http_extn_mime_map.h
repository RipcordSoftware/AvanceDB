#ifndef HTTP_EXTN_MIME_MAP_H
#define	HTTP_EXTN_MIME_MAP_H

#include <unordered_map>

class HttpExtnMimeMap {        
public:
    
    static const std::string& lookup(const std::string& extn);
    
private:
    
    HttpExtnMimeMap() = delete;    
    HttpExtnMimeMap(const HttpExtnMimeMap&) = delete;
    HttpExtnMimeMap(HttpExtnMimeMap&&) = delete;
    HttpExtnMimeMap& operator =(HttpExtnMimeMap&&) = delete;
    
    static std::unordered_map<std::string, std::string> extn_map_;
    static std::string empty_;
};

#endif	/* HTTP_EXTN_MIME_MAP_H */

