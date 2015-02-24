#ifndef FUTONWEBSITERESOURCE_H
#define	FUTONWEBSITERESOURCE_H

#include <string>

#include "httpserver.hpp"
#include "httpserver/http_response.hpp"

class FutonWebSiteResource : public httpserver::http_resource<FutonWebSiteResource> {
public:
    FutonWebSiteResource(const std::string& web_root) : web_root_(web_root) {}
    
    void render(const httpserver::http_request& req, httpserver::http_response** res);
    
private:
    std::string web_root_;

};

#endif	/* FUTONWEBSITERESOURCE_H */

