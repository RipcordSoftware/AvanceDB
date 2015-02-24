#ifndef HTTP_SERVER_CUSTOM_RESOURCE_H
#define	HTTP_SERVER_CUSTOM_RESOURCE_H

#include <functional>
#include <string>
#include <tuple>

#include "httpserver.hpp"
#include "httpserver/http_response.hpp"

class HttpServerCustomResource : public httpserver::http_resource<HttpServerCustomResource> {
public:
    HttpServerCustomResource(std::function<std::tuple<std::string, int, std::string> (const httpserver::http_request&)> func) : 
        func_(func) {}         
    
    void render(const httpserver::http_request& req, httpserver::http_response** res) {
        auto response = func_(req);
        *res = new httpserver::http_response(httpserver::http_response_builder(std::get<0>(response), std::get<1>(response), std::get<2>(response)).string_response());
    }
    
private:
    const std::function<std::tuple<std::string, int, std::string> (const httpserver::http_request&)> func_;

};

#endif	/* HTTP_SERVER_CUSTOM_RESOURCE_H */

