#include "http_server.h"

#include <iostream>

#include "libhttpserver.h"

void HttpServer::Start() {
    auto server = rs::httpserver::HttpServer::Create("0.0.0.0", 5994);
    
    auto func = [](rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
        auto uri = request->getUri();
        
        if (uri.find("/_utils") == 0) {
            if (uri == "/_utils" || uri == "/_utils/") {
                response->Redirect("/_utils/index.html");
            } else {
                auto contentType = rs::httpserver::MimeTypes::GetType(uri);
                if (contentType) {
                    uri = "www" + uri;

                    rs::httpserver::FileStream stream(uri);
                    if (stream) {
                        auto lastModifiedTime = stream.getLastModifiedTime();
                        if (lastModifiedTime) {
                            auto etag = boost::lexical_cast<std::string>(lastModifiedTime.get());
                            
                            if (etag == request->getIfNoneMatch()) {
                              response->setStatusCode(304).setStatusDescription("Not Modified").Send();
                            } else {
                                response->setETag(etag);
                            }

                        }
                        if (!response->HasResponded()) {
                            response->setContentType(contentType.get()).Send(stream);
                        }
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
}