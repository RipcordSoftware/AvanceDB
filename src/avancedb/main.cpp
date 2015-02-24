#include <iostream>
#include <string>
#include <unordered_map>

#include <boost/filesystem.hpp>

#include <httpserver.hpp>
#include <httpserver/http_response.hpp>

using namespace httpserver;

std::unordered_map<std::string, std::string> extnMap = { { ".html", "text/html" }, { ".htm", "text/html" }, { ".css", "text/css" }, { ".js", "text/javascript" }, { ".png", "image/png" }, { ".gif", "image/gif" }, { ".jpg", "image/jpeg" } };

class futon_resource : public http_resource<futon_resource> {    
    std::string webRoot_;
public:
    futon_resource(std::string webRoot) : webRoot_(webRoot) {
    }
    
    void render(const http_request& req, http_response** res) {
        auto uri = req.get_path();        
        
        if (uri == "/_utils" || uri == "/_utils/") {            
            *res = new http_response(http_response_builder(webRoot_ + "/_utils/index.html", 200, "text/html").file_response());
        } else {
            auto path = boost::filesystem::path(webRoot_);
            path /= uri;
            
            auto extn = path.extension();
            
            auto ct = extnMap.find(extn.native());
            if (ct != extnMap.end()) {
                *res = new http_response(http_response_builder(path.native(), 200, ct->second).file_response());
            } else {
                *res = new http_response(http_response_builder("Not found", 404, "text/plain").string_response());
            }
        }
    }
};

int main(int argc, char** argv) {
    
    auto webRoot = boost::filesystem::path(argv[0]).parent_path();
    webRoot /= boost::filesystem::path("www");
    
    futon_resource fr(webRoot.native());
    
    webserver ws = create_webserver(8080).max_threads(8);
    ws.register_resource("/_utils/", &fr, true);
       
    ws.start(true);

    return 0;
}