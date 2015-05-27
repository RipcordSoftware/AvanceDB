#ifndef HTTP_SERVER_H
#define	HTTP_SERVER_H

#include "libhttpserver.h"
#include "rest_server.h"

class HttpServer {
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

