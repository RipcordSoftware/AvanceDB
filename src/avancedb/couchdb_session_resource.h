#ifndef COUCHDB_SESSION_RESOURCE_H
#define	COUCHDB_SESSION_RESOURCE_H

#include "httpserver.hpp"
#include "httpserver/http_response.hpp"

class CouchDBSessionResource : public httpserver::http_resource<CouchDBSessionResource> {
public:
    CouchDBSessionResource() : 
        body_("{\"ok\":true,\"userCtx\":{\"name\":null,\"roles\":[\"_admin\"]},\"info\":{\"authentication_db\":\"_users\",\"authentication_handlers\":[\"oauth\",\"cookie\",\"default\"],\"authenticated\":\"default\"}}"),
        response_builder_(body_, 200, "application/javascript") {}         
    
    void render(const httpserver::http_request& req, httpserver::http_response** res) {
        *res = new httpserver::http_response(response_builder_.string_response());
    }
    
private:
    std::string body_;
    httpserver::http_response_builder response_builder_;

};

#endif	/* COUCHDB_SESSION_RESOURCE_H */

