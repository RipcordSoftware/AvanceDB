#include "rest_server.h"

void RestServer::RouteRequest(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    auto uri = request->getUri();
    
    if (uri == "/_session") {
        response->setContentType("application/javascript").Send("{\"ok\":true,\"userCtx\":{\"name\":null,\"roles\":[\"_admin\"]},\"info\":{\"authentication_db\":\"_users\",\"authentication_handlers\":[\"oauth\",\"cookie\",\"default\"],\"authenticated\":\"default\"}}");
    } else if (uri == "/_all_dbs") {            
        response->setContentType("application/javascript").Send("[]");
    } else if (uri == "/") {
        response->setContentType("application/javascript").Send("{\"couchdb\":\"Welcome\",\"uuid\":\"a2db86472466bcd02e84ac05a6c86185\",\"version\":\"1.6.1\",\"vendor\":{\"version\":\"1.6.1\",\"name\":\"The Apache Software Foundation\"}}");
    }
}