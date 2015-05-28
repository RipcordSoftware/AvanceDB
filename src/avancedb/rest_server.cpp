#include "rest_server.h"

#include <sstream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "json_stream.h"

#define REGEX_DBNAME R"(_?[a-z][a-z0-9_\$\+\-\(\)]+)"
#define REGEX_DBNAME_GROUP "/(?<db>" REGEX_DBNAME ")"

RestServer::RestServer() {
    router_.Add("HEAD", REGEX_DBNAME_GROUP "/?", boost::bind(&RestServer::HeadDatabase, this, _1, _2, _3));
    
    router_.Add("DELETE", REGEX_DBNAME_GROUP "/?", boost::bind(&RestServer::DeleteDatabase, this, _1, _2, _3));
    
    router_.Add("PUT", REGEX_DBNAME_GROUP "/?", boost::bind(&RestServer::PutDatabase, this, _1, _2, _3));
    
    router_.Add("GET", "/_active_tasks", boost::bind(&RestServer::GetActiveTasks, this, _1, _2, _3));
    router_.Add("GET", "/_uuids", boost::bind(&RestServer::GetUuids, this, _1, _2, _3));
    router_.Add("GET", "/_session", boost::bind(&RestServer::GetSession, this, _1, _2, _3));
    router_.Add("GET", "/_all_dbs", boost::bind(&RestServer::GetAllDbs, this, _1, _2, _3));    
    router_.Add("GET", REGEX_DBNAME_GROUP "/_all_docs", boost::bind(&RestServer::GetDatabaseAllDocs, this, _1, _2, _3));
    router_.Add("GET", REGEX_DBNAME_GROUP "/?", boost::bind(&RestServer::GetDatabase, this, _1, _2, _3));
    router_.Add("GET", "/\\_config/query_servers/?", boost::bind(&RestServer::GetConfigQueryServers, this, _1, _2, _3));
    router_.Add("GET", "/\\_config/native_query_servers/?", boost::bind(&RestServer::GetConfigNativeQueryServers, this, _1, _2, _3));
    router_.Add("GET", "/", boost::bind(&RestServer::GetSignature, this, _1, _2, _3));
    
    databases_.AddDatabase("_replicator");
    databases_.AddDatabase("_users");
}

void RestServer::RouteRequest(rs::httpserver::socket_ptr, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    router_.Match(request, response);
}

bool RestServer::GetActiveTasks(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType("application/javascript").Send("[]");
    return true;
}

bool RestServer::GetSession(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType("application/javascript").Send("{\"ok\":true,\"userCtx\":{\"name\":null,\"roles\":[\"_admin\"]},\"info\":{\"authentication_db\":\"_users\",\"authentication_handlers\":[\"oauth\",\"cookie\",\"default\"],\"authenticated\":\"default\"}}");
    return true;
}

bool RestServer::GetAllDbs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    auto dbs = databases_.GetDatabases();
    
    std::stringstream stream;
    stream << "[";
    for (int i = 0; i < dbs.size(); ++i) {
        stream << (i > 0 ? "," : "") << "\"" << dbs[i] << "\"";
    }
    stream << "]";
    
    response->setContentType("application/javascript").Send(stream.str());
    return true;
}

bool RestServer::GetSignature(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType("application/javascript").Send("{\"couchdb\":\"Welcome\",\"uuid\":\"a2db86472466bcd02e84ac05a6c86185\",\"version\":\"1.6.1\",\"vendor\":{\"version\":\"1.6.1\",\"name\":\"The Apache Software Foundation\"}}");
    return true;
}

bool RestServer::GetUuids(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    int count = 1;
    
    if (request->getQueryString().IsKey("count")) {
        auto countParam = request->getQueryString().getValue("count");
        if (countParam.length() > 0) {
            count = boost::lexical_cast<int>(countParam);
        }
    }
    
    std::stringstream stream;    
    stream << std::hex << "{\"uuids\":[";
    
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    for (int i = 0; i < count; ++i) {                
        stream << (i > 0 ? "," : "") << "\"";
        
        stream.width(2);
        auto uuid = gen();
        for (auto iter = uuid.begin(); iter != uuid.end(); iter++) {
            stream << static_cast<int>(*iter);
        }
        stream.width(0);
        
        stream << "\"";
    }
    
    stream << "]}";
    
    response->setContentType("application/javascript").Send(stream.str());    
    return true;
}

bool RestServer::HeadDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto iter = args.find("db");

    bool found = iter != args.cend() && databases_.IsDatabase(iter->second);
    if (found) {
        response->setContentType("application/javascript").Send();            
    }
    
    return found;            
}

bool RestServer::GetDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto argsIter = args.find("db");
    
    bool found = argsIter != args.cend();
    if (found) {
        auto db = databases_.GetDatabase(argsIter->second);
        
        found = !!db;
        if (found) {
            JsonStream stream;
            stream.Append("committed_update_seq", db->CommitedUpdateSequence());
            stream.Append("compact_running", false);
            stream.Append("data_size", db->DataSize());
            stream.Append("db_name", argsIter->second);
            stream.Append("disk_format_version", 6);
            stream.Append("disk_size", db->DiskSize());
            stream.Append("doc_count", db->DocCount());
            stream.Append("doc_del_count", db->DocDelCount());
            stream.Append("instance_start_time", db->InstanceStartTime());
            stream.Append("purge_seq", db->PurgeSequence());
            stream.Append("update_seq", db->UpdateSequence());
            
            response->setContentType("application/javascript").Send(stream.Flush());
        }
    }
    
    return found;
}

// TODO: correctly handle the error cases here
bool RestServer::PutDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto argsIter = args.find("db");
    
    if (argsIter != args.cend() && !databases_.IsDatabase(argsIter->second)) {
        created = databases_.AddDatabase(argsIter->second);
    }
    
    if (created) {
        response->setStatusCode(201).setContentType("application/javascript").Send("{\"ok\":true}");
    }
    
    return created;
}

// TODO: correctly handle the error cases here
bool RestServer::DeleteDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool deleted = false;
    auto argsIter = args.find("db");
    
    if (argsIter != args.cend()) {
        deleted = databases_.RemoveDatabase(argsIter->second);
    }
    
    if (deleted) {
        response->setContentType("application/javascript").Send("{\"ok\":true}");
    }
    
    return deleted;
}

bool RestServer::GetDatabaseAllDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    response->setContentType("application/javascript").Send("{\"offset\":0,\"rows\":[],\"total_rows\":0}");
}

bool RestServer::GetConfigQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType("application/javascript").Send("{\"javascript\":\"libjsapi\"}");
}

bool RestServer::GetConfigNativeQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType("application/javascript").Send("{}");
}