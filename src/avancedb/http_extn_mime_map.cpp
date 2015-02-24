#include "http_extn_mime_map.h"

std::unordered_map<std::string, std::string> HttpExtnMimeMap::extn_map_ = 
    { { ".html", "text/html" }, { ".htm", "text/html" }, { ".css", "text/css" }, { ".js", "text/javascript" }, { ".png", "image/png" }, { ".gif", "image/gif" }, { ".jpg", "image/jpeg" } };

std::string HttpExtnMimeMap::empty_;

const std::string& HttpExtnMimeMap::lookup(const std::string& extn) {
    auto pair = extn_map_.find(extn);
    if (pair != extn_map_.end()) {
        return pair->second;
    } else {
        return empty_;
    }
}