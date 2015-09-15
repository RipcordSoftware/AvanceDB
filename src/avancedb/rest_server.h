#ifndef REST_SERVER_H
#define	REST_SERVER_H

#include "libhttpserver.h"
#include "libscriptobject.h"

#include "types.h"
#include "databases.h"
#include "uuid_helper.h"

class RestServer final {
public:
    
    RestServer();
    void RouteRequest(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response);
    
private:
    
    using Callback = bool(RestServer::*)(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    void AddRoute(const char* method, const char* re, Callback func);
    
    bool HeadDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool HeadDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
        
    bool GetActiveTasks(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetUuids(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetSession(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDatabaseAllDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostDatabaseAllDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetAllDbs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetConfig(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetConfigQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetConfigNativeQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetSignature(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool PutDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PutDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool PostDatabaseBulkDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostDatabaseRevsDiff(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostEnsureFullCommit(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool DeleteDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool DeleteDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool GetLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PutLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool DeleteLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    database_ptr GetDatabase(const rs::httpserver::RequestRouter::CallbackArgs&);
    const char* GetDatabaseName(const rs::httpserver::RequestRouter::CallbackArgs&);
    const std::string& GetParameter(const char* param, const rs::httpserver::QueryString&, bool throwIfMissing = false);
    const char* GetParameter(const char* param, const rs::httpserver::RequestRouter::CallbackArgs&);
    rs::scriptobject::ScriptObjectPtr GetJsonBody(rs::httpserver::request_ptr request, bool useCachedObjectKeys = true);
    
    rs::httpserver::RequestRouter router_;        
    Databases databases_;

};

#endif	/* REST_SERVER_H */

