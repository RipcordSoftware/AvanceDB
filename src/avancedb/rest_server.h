/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015 Ripcord Software
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

#ifndef REST_SERVER_H
#define REST_SERVER_H

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
    
    bool HeadServer(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool HeadDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool HeadDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool HeadDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
        
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
    bool GetDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDesignDocumentView(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool PutDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PutDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PutDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool PostDatabaseBulkDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostDatabaseRevsDiff(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostEnsureFullCommit(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PostTempView(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool DeleteDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool DeleteDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool DeleteDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);

    bool GetLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PutLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool DeleteLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    bool HeadDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool PutDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool GetDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    bool DeleteDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response);
    
    database_ptr GetDatabase(const rs::httpserver::RequestRouter::CallbackArgs&);
    const char* GetDatabaseName(const rs::httpserver::RequestRouter::CallbackArgs&);
    const std::string& GetParameter(const char* param, const rs::httpserver::QueryString&, bool throwIfMissing = false);
    const char* GetParameter(const char* param, const rs::httpserver::RequestRouter::CallbackArgs&);
    rs::scriptobject::ScriptObjectPtr GetRequestBody(rs::httpserver::request_ptr request, bool useCachedObjectKeys = true);
    
    rs::httpserver::RequestRouter router_;        
    Databases databases_;

};

#endif	/* REST_SERVER_H */

