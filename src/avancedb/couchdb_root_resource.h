#ifndef COUCHDB_ROOT_RESOURCE_H
#define	COUCHDB_ROOT_RESOURCE_H

#include "httpserver.hpp"
#include "httpserver/http_response.hpp"

class CouchDBRootResource : public httpserver::http_resource<CouchDBRootResource> {
public:
    CouchDBRootResource() : 
        body_("{\"couchdb\":\"Welcome\",\"uuid\":\"a2db86472466bcd02e84ac05a6c86185\",\"version\":\"1.6.1\",\"vendor\":{\"version\":\"1.6.1\",\"name\":\"The Apache Software Foundation\"}}"),
        response_builder_(body_, 200, "application/javascript") {}         
    
    void render(const httpserver::http_request& req, httpserver::http_response** res) {
        *res = new httpserver::http_response(response_builder_.string_response());
    }
    
private:
    std::string body_;
    httpserver::http_response_builder response_builder_;

};

#endif	/* COUCHDB_ROOT_RESOURCE_H */