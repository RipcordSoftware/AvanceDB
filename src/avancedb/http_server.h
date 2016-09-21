/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2016 Ripcord Software
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

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "libhttpserver.h"
#include "rest_server.h"

class HttpServer final {
public:
    
    HttpServer(const char* address = "0.0.0.0", int port = 5994);
        
    void Start();
    
private:
    
    void RequestCallback(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response);    
    void HandleUtilsRequest(rs::httpserver::request_ptr request, rs::httpserver::response_ptr response);
    
    void InternalErrorResponse(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) noexcept;
    
    rs::httpserver::server_ptr server_;
    RestServer rest_;

};

#endif	/* HTTP_SERVER_H */

