#include <iostream>

#include <boost/filesystem.hpp>

#include "httpserver.hpp"

#include "futon_web_site_resource.h"
#include "couchdb_root_resource.h"
#include "couchdb_session_resource.h"
#include "http_server_custom_resource.h"

using namespace httpserver;

int main(int argc, char** argv) {
    
    auto webRoot = boost::filesystem::path(argv[0]).parent_path();
    webRoot /= boost::filesystem::path("www");
    
    FutonWebSiteResource futon_res(webRoot.native());
    CouchDBRootResource cdbroot_res;
    CouchDBSessionResource cdbsession_res;    
    
    // a stub handler for the /_all_dbs REST call
    HttpServerCustomResource alldbs_res([](const httpserver::http_request&) { return std::make_tuple("[]", 200, "application/javascript"); });
    
    webserver ws = create_webserver(8080).max_threads(8);
    ws.register_resource("/_utils/", &futon_res, true);
    ws.register_resource("/_session", &cdbsession_res, false);
    ws.register_resource("/_all_dbs", &alldbs_res, false);
    ws.register_resource("/", &cdbroot_res, false);    
       
    ws.start(true);

    return 0;
}