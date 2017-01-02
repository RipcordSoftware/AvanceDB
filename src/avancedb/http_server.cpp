/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2017 Ripcord Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "http_server.h"

#include <iostream>

#include "http_server_exception.h"
#include "http_server_log.h"

HttpServer::HttpServer(const char* address, int port) : server_(rs::httpserver::HttpServer::Create(address, port)) {
}

void HttpServer::Start() {    
    server_->Start(boost::bind(&HttpServer::RequestCallback, this, _1, _2, _3));
}

void HttpServer::RequestCallback(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    
    auto start = boost::chrono::system_clock::now();
    
    try {
        if (request->getUri().find("/_utils") == 0) {
            HandleUtilsRequest(request, response);
            
            if (!response->HasResponded()) {
                response->setStatusCode(404).setStatusDescription("Not Found").Send();
            }
        } else {
            rest_.RouteRequest(socket, request, response);
        }                
    } catch (const HttpServerException& ex) {
        if (!response->HasResponded()) {
            try {
                response->setContentType(ex.ContentType()).setStatusCode(ex.StatusCode()).setStatusDescription(ex.Description()).Send(ex.Body());
            } catch (...) {}
        }
    } catch (const boost::exception& ex) {
        if (!response->HasResponded()) {
            InternalErrorResponse(socket, request, response);
        }
    } catch (const std::exception& ex) {
        if (!response->HasResponded()) {
            InternalErrorResponse(socket, request, response);
        }
    }           
    
    if (response->HasResponded()) {
        auto duration = boost::chrono::system_clock::now() - start;
        auto durationMS = boost::chrono::duration_cast<boost::chrono::milliseconds>(duration);

        HttpServerLog::Append(socket, request, response, boost::chrono::system_clock::to_time_t(start), durationMS.count());
    }
}

void HttpServer::HandleUtilsRequest(rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    auto uri = request->getUri();

    if (uri == "/_utils" || uri == "/_utils/") {
        response->Redirect("/_utils/index.html");
    } else {
        auto contentType = rs::httpserver::MimeTypes::GetExtensionType(uri);
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
    try {
        response->setContentType("text/plain").setStatusCode(500).setStatusDescription("Internal Server Error").Send();
    } catch (...) {}
}