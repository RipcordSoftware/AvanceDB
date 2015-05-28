#ifndef REST_SERVER_H
#define	REST_SERVER_H

#include "libhttpserver.h"

#include "databases.h"

class RestServer final {
public:
    
    RestServer();
    void RouteRequest(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response);
    
private:
    
    bool HeadDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
        
    bool GetActiveTasks(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetUuids(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetSession(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDatabaseAllDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetAllDbs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetConfigQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetConfigNativeQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetSignature(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool PutDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool DeleteDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    rs::httpserver::RequestRouter router_;        
    Databases databases_;

};

#endif	/* REST_SERVER_H */

