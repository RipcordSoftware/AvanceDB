#include <boost/filesystem.hpp>

#include "futon_web_site_resource.h"
#include "http_extn_mime_map.h"

using namespace httpserver;

void FutonWebSiteResource::render(const http_request& req, http_response** res) {
    auto uri = req.get_path();        

    if (uri == "/_utils" || uri == "/_utils/") {            
        *res = new http_response(http_response_builder(web_root_ + "/_utils/index.html", 200, "text/html").file_response());
    } else {
        auto path = boost::filesystem::path(web_root_);
        path /= uri;

        auto extn = path.extension();

        auto ct = !extn.empty() ? HttpExtnMimeMap::lookup(extn.native()) : std::string();
        if (!ct.empty()) {
            *res = new http_response(http_response_builder(path.native(), 200, ct).file_response());
        } else {
            *res = new http_response(http_response_builder("Not found", 404, "text/plain").string_response());
        }
    }
}