#include <iostream>

#include "libhttpserver.h"

int main() {
    auto server = rs::httpserver::HttpServer::Create("0.0.0.0", 5994);
    
    auto func = [](rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
        auto uri = request->getUri();
        
        if (uri.find("/_utils") == 0) {
            if (uri == "/_utils") {
                response->setHeader(rs::httpserver::Headers::Location, "/_utils/index.html").setStatusCode(302).Send();
            } else {
                auto contentType = rs::httpserver::MimeTypes::GetType(uri);
                if (contentType) {                            
                    uri = "www" + uri;

                    rs::httpserver::FileStream stream(uri);
                    if (stream) {
                        response->setContentType(contentType.get()).Send(stream);
                    }
                }
            }
        } else if (uri == "/_session") {
            response->setContentType("application/javascript").Send("{\"ok\":true,\"userCtx\":{\"name\":null,\"roles\":[\"_admin\"]},\"info\":{\"authentication_db\":\"_users\",\"authentication_handlers\":[\"oauth\",\"cookie\",\"default\"],\"authenticated\":\"default\"}}");
        } else if (uri == "/_all_dbs") {            
            response->setContentType("application/javascript").Send("[]");
        } else if (uri == "/") {
            response->setContentType("application/javascript").Send("{\"couchdb\":\"Welcome\",\"uuid\":\"a2db86472466bcd02e84ac05a6c86185\",\"version\":\"1.6.1\",\"vendor\":{\"version\":\"1.6.1\",\"name\":\"The Apache Software Foundation\"}}");
        }
    };    

    server->Start(func);
    
    return 0;
}