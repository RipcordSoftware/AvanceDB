#include <iostream>

#include <boost/filesystem.hpp>

#include "httpserver.hpp"

#include "futon_web_site_resource.h"
#include "couchdb_root_resource.h"
#include "couchdb_session_resource.h"

using namespace httpserver;

int main(int argc, char** argv) {
    
    auto webRoot = boost::filesystem::path(argv[0]).parent_path();
    webRoot /= boost::filesystem::path("www");
    
    FutonWebSiteResource fr(webRoot.native());
    CouchDBRootResource cdbrr;
    CouchDBSessionResource cdbsr;
    
    webserver ws = create_webserver(8080).max_threads(8);
    ws.register_resource("/_utils/", &fr, true);
    ws.register_resource("/_session", &cdbsr, false);    
    ws.register_resource("/", &cdbrr, false);    
       
    ws.start(true);

    return 0;
}