#ifndef REST_SERVER_H
#define	REST_SERVER_H

#include "libhttpserver.h"

class RestServer {
public:
    
    void RouteRequest(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response);
    
private:

};

#endif	/* REST_SERVER_H */

