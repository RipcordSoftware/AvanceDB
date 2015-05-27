#include "http_server.h"

#include <iostream>

HttpServer::HttpServer(const char* address, int port) : server_(rs::httpserver::HttpServer::Create(address, port)) {
}

void HttpServer::Start() {    
    server_->Start(boost::bind(&HttpServer::RequestCallback, this, _1, _2, _3));
}

void HttpServer::RequestCallback(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    
    try {
        rest_.RouteRequest(socket, request, response);

        if (!response->HasResponded() && request->getUri().find("/_utils") == 0) {
            HandleUtilsRequest(request, response);
        }
    } catch (const boost::exception& ex) {
        InternalErrorResponse(socket, request, response);
    } catch (const std::exception& ex) {
        InternalErrorResponse(socket, request, response);
    }
}

void HttpServer::HandleUtilsRequest(rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    auto uri = request->getUri();

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
}

void HttpServer::InternalErrorResponse(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) noexcept {
    response->setContentType("text/plain").setStatusCode(500).setStatusDescription("Internal Server Error").Send();
}