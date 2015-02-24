#include <iostream>

#include <boost/filesystem.hpp>

#include "httpserver.hpp"

#include "futon_web_site_resource.h"

using namespace httpserver;

int main(int argc, char** argv) {
    
    auto webRoot = boost::filesystem::path(argv[0]).parent_path();
    webRoot /= boost::filesystem::path("www");
    
    FutonWebSiteResource fr(webRoot.native());
    
    webserver ws = create_webserver(8080).max_threads(8);
    ws.register_resource("/_utils/", &fr, true);
       
    ws.start(true);

    return 0;
}