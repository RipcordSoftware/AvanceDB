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

#include "rest_server.h"

#include <sstream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <boost/format.hpp>

#include "content_types.h"
#include "json_stream.h"
#include "rest_exceptions.h"
#include "database.h"
#include "document.h"
#include "get_all_documents_options.h"
#include "post_all_documents_options.h"
#include "script_object_response_stream.h"
#include "rest_config.h"
#include "document_revision.h"
#include "map_reduce_result.h"
#include "map_reduce_results_iterator.h"
#include "get_view_options.h"

#include "libscriptobject_gason.h"
#include "libscriptobject_msgpack.h"
#include "base64_helper.h"

#define REGEX_DBNAME R"(_?[a-z][a-z0-9_\$\+\-\(\)]+)"
#define REGEX_DBNAME_GROUP "/(?<db>" REGEX_DBNAME ")"

#define REGEX_DOCID R"([a-zA-Z0-9\$\+\-\(\)\:\.\~][a-zA-Z0-9_\$\+\-\(\)\:\.\~]*)"
#define REGEX_ATTACHMENT_NAME R"(.*)"
#define REGEX_DESIGNID REGEX_DOCID
#define REGEX_VIEWID REGEX_DOCID
#define REGEX_DOCID_GROUP "/+(?<id>" REGEX_DOCID ")"
#define REGEX_DESIGNID_GROUP "/+(?<designid>" REGEX_DESIGNID ")"
#define REGEX_VIEWID_GROUP "/+(?<viewid>" REGEX_VIEWID ")"
#define REGEX_ATTACHMENT_NAME_GROUP "/+(?<attname>" REGEX_ATTACHMENT_NAME ")"

RestServer::RestServer() {
    AddRoute("HEAD", REGEX_DBNAME_GROUP "/{0,}$", &RestServer::HeadDatabase);   
    AddRoute("HEAD", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP, &RestServer::HeadDocument);
    AddRoute("HEAD", REGEX_DBNAME_GROUP "/+_design" REGEX_DESIGNID_GROUP, &RestServer::HeadDesignDocument);
    AddRoute("HEAD", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP REGEX_ATTACHMENT_NAME_GROUP, &RestServer::HeadDocumentAttachment);
    AddRoute("HEAD", "/+", &RestServer::HeadServer);
    
    AddRoute("DELETE", REGEX_DBNAME_GROUP "/+_local" REGEX_DOCID_GROUP, &RestServer::DeleteLocalDocument);
    AddRoute("DELETE", REGEX_DBNAME_GROUP "/{0,}$", &RestServer::DeleteDatabase);
    AddRoute("DELETE", REGEX_DBNAME_GROUP "/+_design" REGEX_DESIGNID_GROUP, &RestServer::DeleteDesignDocument);
    AddRoute("DELETE", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP REGEX_ATTACHMENT_NAME_GROUP, &RestServer::DeleteDocumentAttachment);
    AddRoute("DELETE", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP, &RestServer::DeleteDocument);
    
    AddRoute("PUT", REGEX_DBNAME_GROUP "/+_local" REGEX_DOCID_GROUP, &RestServer::PutLocalDocument);
    AddRoute("PUT", REGEX_DBNAME_GROUP "/+_design" REGEX_DESIGNID_GROUP, &RestServer::PutDesignDocument);
    AddRoute("PUT", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP REGEX_ATTACHMENT_NAME_GROUP, &RestServer::PutDocumentAttachment);
    AddRoute("PUT", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP, &RestServer::PutDocument);
    AddRoute("PUT", REGEX_DBNAME_GROUP "/{0,}$", &RestServer::PutDatabase);
    
    AddRoute("POST", REGEX_DBNAME_GROUP "/+_all_docs", &RestServer::PostDatabaseAllDocs);
    AddRoute("POST", REGEX_DBNAME_GROUP "/+_bulk_docs", &RestServer::PostDatabaseBulkDocs);
    AddRoute("POST", REGEX_DBNAME_GROUP "/+_revs_diff", &RestServer::PostDatabaseRevsDiff);
    AddRoute("POST", REGEX_DBNAME_GROUP "/+_ensure_full_commit", &RestServer::PostEnsureFullCommit);
    AddRoute("POST", REGEX_DBNAME_GROUP "/+_temp_view", &RestServer::PostTempView);
    AddRoute("POST", REGEX_DBNAME_GROUP "/{0,}$", &RestServer::PostDatabase);
    
    AddRoute("GET", "/+_active_tasks/{0,}$", &RestServer::GetActiveTasks);
    AddRoute("GET", "/+_uuids/{0,}$", &RestServer::GetUuids);
    AddRoute("GET", "/+_session/{0,}$", &RestServer::GetSession);
    AddRoute("GET", "/+_all_dbs/{0,}$", &RestServer::GetAllDbs);    
    AddRoute("GET", "/+_config/query_servers/{0,}$", &RestServer::GetConfigQueryServers);
    AddRoute("GET", "/+_config/native_query_servers/{0,}$", &RestServer::GetConfigNativeQueryServers);
    AddRoute("GET", "/+_config/{0,}$", &RestServer::GetConfig);
    AddRoute("GET", REGEX_DBNAME_GROUP "/+_local" REGEX_DOCID_GROUP, &RestServer::GetLocalDocument);
    AddRoute("GET", REGEX_DBNAME_GROUP "/+_design" REGEX_DESIGNID_GROUP "/_view" REGEX_VIEWID_GROUP, &RestServer::GetDesignDocumentView);
    AddRoute("GET", REGEX_DBNAME_GROUP "/+_design" REGEX_DESIGNID_GROUP, &RestServer::GetDesignDocument);
    AddRoute("GET", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP REGEX_ATTACHMENT_NAME_GROUP, &RestServer::GetDocumentAttachment);
    AddRoute("GET", REGEX_DBNAME_GROUP REGEX_DOCID_GROUP, &RestServer::GetDocument);
    AddRoute("GET", REGEX_DBNAME_GROUP "/+_all_docs/{0,}$", &RestServer::GetDatabaseAllDocs);
    AddRoute("GET", REGEX_DBNAME_GROUP "/+_revs_limit/{0,}$", &RestServer::GetDatabaseRevsLimit);
    AddRoute("GET", REGEX_DBNAME_GROUP "/{0,}$", &RestServer::GetDatabase);
    AddRoute("GET", "/{0,}$", &RestServer::GetSignature);
    
    databases_.AddDatabase("_replicator");
    databases_.AddDatabase("_users");
}

void RestServer::AddRoute(const char* method, const char* re, Callback func) {
    router_.Add(method, re, boost::bind(func, this, _1, _2, _3));
}

void RestServer::RouteRequest(rs::httpserver::socket_ptr, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response) {
    router_.Match(request, response);
    
    if (!response->HasResponded()) {
        response->setContentType(ContentTypes::textPlain).setStatusCode(404).setStatusDescription("Not Found").Send(R"({"error":"not_found","reason":"no_db_file"})");
    }
}

bool RestServer::GetActiveTasks(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType(ContentTypes::applicationJson).Send("[]");
    return true;
}

bool RestServer::GetSession(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType(ContentTypes::applicationJson).Send(R"({"ok":true,"userCtx":{"name":null,"roles":["_admin"]},"info":{"authentication_db":"_users","authentication_handlers":["oauth","cookie","default"],"authenticated":"default"}})");
    return true;
}

bool RestServer::GetAllDbs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    auto dbs = databases_.GetDatabases();
    
    JsonStream stream{JsonStream::ContextType::Array};
    for (int i = 0; i < dbs.size(); ++i) {
        stream.Append(dbs[i]);
    }
    
    response->setContentType(ContentTypes::applicationJson).Send(stream.Flush());
    return true;
}

bool RestServer::GetSignature(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType(ContentTypes::applicationJson).Send(R"({"couchdb":"Welcome","avancedb":"Welcome","uuid":"a2db86472466bcd02e84ac05a6c86185","version":"1.6.1","vendor":{"version":"0.2.0","name":"Ripcord Software"}})");
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
    
    if (count > 1000) {
        throw UuidCountLimit();
    }
    
    JsonStream stream;
    stream.PushContext(JsonStream::ContextType::Array, "uuids");
    
    UuidHelper::UuidGenerator gen;
    for (int i = 0; i < count; ++i) {                
        UuidHelper::UuidString uuidString;
        UuidHelper::FormatUuid(gen(), uuidString);
        
        stream.Append(uuidString);
    }
    
    response->setContentType(ContentTypes::applicationJson).Send(stream.Flush());    
    return true;
}

bool RestServer::HeadDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool found = false;
    
    auto name = GetDatabaseName(args);
    if (name != nullptr && databases_.IsDatabase(name)) {
        response->setContentType(ContentTypes::applicationJson).Send();            
        found = true;
    }
    
    return found;            
}

bool RestServer::HeadServer(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {        
    response->setContentType(ContentTypes::applicationJson).Send();    
    return true;            
}

bool RestServer::GetDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool found = false;

    auto name = GetDatabaseName(args);
    if (name != nullptr) {
        auto db = databases_.GetDatabase(name);
        
        found = !!db;
        if (found) {
            JsonStream stream;
            stream.Append("committed_update_seq", db->CommitedUpdateSequence());
            stream.Append("compact_running", false);
            stream.Append("data_size", db->DataSize());
            stream.Append("db_name", name);
            stream.Append("disk_format_version", 6);
            stream.Append("disk_size", db->DiskSize());
            stream.Append("doc_count", db->DocCount());
            stream.Append("doc_del_count", db->DocDelCount());
            stream.Append("instance_start_time", db->InstanceStartTime());
            stream.Append("purge_seq", db->PurgeSequence());
            stream.Append("update_seq", db->UpdateSequence());
            
            response->setContentType(ContentTypes::applicationJson).Send(stream.Flush());
        } else {
            throw MissingDatabase();
        }
    }
    
    return found;
}

bool RestServer::PutDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    
    auto name = GetDatabaseName(args);
    if (name != nullptr) {
        if (std::strlen(name) <= 0 || name[0] == '_') {
            throw InvalidDatabaseName();
        }
        
        if (databases_.IsDatabase(name)) {
            throw DatabaseAlreadyExists();
        }

        created = databases_.AddDatabase(name);
        
        if (created) {
            response->setStatusCode(201).setContentType(ContentTypes::applicationJson).Send(R"({"ok":true})");
        }
    }
    
    return created;
}

bool RestServer::PostDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto db = GetDatabase(args);
    if (!!db) {
        UuidHelper::UuidString uuidString;
        auto obj = GetRequestBody(request);

        if (!!obj) {                      
            auto id = obj->getString("_id", false);
            if (!id) {
                UuidHelper::UuidGenerator gen;
                UuidHelper::FormatUuid(gen(), uuidString);
                id = uuidString;
            }
            
            auto doc = db->SetDocument(id, obj);
            
            auto rev = doc->getRev();
            
            JsonStream stream;
            stream.Append("ok", true);
            stream.Append("id", doc->getId());
            stream.Append("rev", rev);

            response->setStatusCode(201).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).Send(stream.Flush());

            created = true;
        }
    }
    
    return created;
}

bool RestServer::DeleteDatabase(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool deleted = false;
    
    auto name = GetDatabaseName(args);    
    if (name != nullptr) {                
        if (std::strlen(name) <= 0 || name[0] == '_') {
            throw InvalidDatabaseName();
        }
        
        deleted = databases_.RemoveDatabase(name);
        
        if (deleted) {
            response->setContentType(ContentTypes::applicationJson).Send(R"({"ok":true})");
        } else {
            throw MissingDatabase();
        }
    }
    
    return deleted;
}

bool RestServer::GetDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool gotDoc = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        auto multiPartAttachments = GetParameter("attachments", request->getQueryString(), false) == "true";
        
        auto doc = db->GetDocument(id);
        auto obj = doc->getObject();
        
        auto rev = doc->getRev();
        
        response->setStatusCode(200).setETag(rev);

        if (multiPartAttachments) {
            auto& stream = response->getMultiResponseStream();
            
            stream.EmitPart(ContentTypes::Utf8::applicationJson);
            ScriptObjectResponseStream<> objStream{stream};
            objStream.Serialize(obj, ScriptObjectResponseStreamAttachmentMode::Follows);
            objStream.Flush(false);
            
            auto attachments = doc->getAttachments();
            for (auto& attachment : attachments) {
                stream.EmitPart(attachment->ContentType().c_str(), attachment->Name().c_str(), attachment->Size());
                stream.Write(attachment->Data(), 0, attachment->Size());
            }

            stream.Flush();
        } else {
            auto& stream = response->setContentType(ContentTypes::Utf8::applicationJson).getResponseStream();
            ScriptObjectResponseStream<> objStream{stream};
            objStream.Serialize(obj, ScriptObjectResponseStreamAttachmentMode::Stub);
            objStream.Flush();
        }
        
        gotDoc = true;
    }
    
    return gotDoc;
}

bool RestServer::GetDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool gotDoc = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("designid", args);
        
        auto doc = db->GetDesignDocument(id);
        auto obj = doc->getObject();
        
        auto rev = doc->getRev();

        auto& stream = response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).getResponseStream();
        ScriptObjectResponseStream<> objStream{stream};
        objStream << obj;
        objStream.Flush();
        
        gotDoc = true;
    }
    
    return gotDoc;
}


bool RestServer::GetDesignDocumentView(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto db = GetDatabase(args);
    if (!!db) {
        response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).Send(R"({"offset":0,"rows":[],"total_rows":0})");
    }    
}

bool RestServer::PutDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        auto obj = GetRequestBody(request);        

        if (!!obj) {
            auto doc = db->SetDocument(id, obj);
            
            auto rev = doc->getRev();
            
            JsonStream stream;
            stream.Append("ok", true);
            stream.Append("id", doc->getId());
            stream.Append("rev", rev);

            response->setStatusCode(201).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).Send(stream.Flush());

            created = true;
        }
    }
    
    return created;
}

bool RestServer::PutDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("designid", args);
        auto obj = GetRequestBody(request);        

        if (!!obj) {
            auto doc = db->SetDesignDocument(id, obj);
            
            auto rev = doc->getRev();
            
            JsonStream stream;
            stream.Append("ok", true);
            stream.Append("id", doc->getId());
            stream.Append("rev", rev);

            response->setStatusCode(201).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).Send(stream.Flush());

            created = true;
        }
    }
    
    return created;
}

bool RestServer::PostDatabaseBulkDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto obj = GetRequestBody(request);
        
        if (obj->getType("docs") != rs::scriptobject::ScriptObjectType::Array) {
            throw InvalidJson{};
        }
        
        auto newEdits = true;
        if (obj->getType("new_edits") == rs::scriptobject::ScriptObjectType::Boolean) {
            newEdits = obj->getBoolean("new_edits");
        }
        
        auto docs = obj->getArray("docs");
        
        auto results = db->PostBulkDocuments(docs, newEdits);

        JsonStream stream{JsonStream::ContextType::Array};
        for (auto result : results) {
            if (newEdits || !result.ok()) {
                stream.PushContext(JsonStream::ContextType::Object);
                stream.Append("id", result.id());

                if (result.ok()) {
                    stream.Append("ok", true);
                    stream.Append("rev", result.rev());
                } else {
                    stream.Append("error", result.error());
                    stream.Append("reason", result.reason());
                }

                stream.PopContext();
            }
        }
        
        response->setStatusCode(201).setContentType(ContentTypes::Utf8::applicationJson).Send(stream.Flush());
        
        created = true;
    }
    
    return created;
}

bool RestServer::PostDatabaseRevsDiff(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool handled = false;
    auto db = GetDatabase(args);
    if (!!db) {
        JsonStream stream;
        
        auto obj = GetRequestBody(request, false);
        
        try {
            auto count = obj->getCount();
            for (decltype(count) i = 0; i < count; ++i) {
                auto id = obj->getName(i);            
                auto revs = obj->getArray(i);

                stream.PushContext(JsonStream::ContextType::Object, id);
                stream.PushContext(JsonStream::ContextType::Array, "missing");

                const char* possibleAncestor = nullptr;
                auto doc = db->GetDocument(id, false);                
                
                auto revsCount = revs->getCount();
                for (decltype(revsCount) j = 0; j < revsCount; ++j) {
                    auto rev = revs->getString(j);
                    
                    if (!doc || std::strcmp(doc->getRev(), rev) != 0) {
                        stream.Append(rev);
                    } else {
                        possibleAncestor = rev;
                    }
                }
                
                stream.PopContext();
                
                if (possibleAncestor) {
                    stream.PushContext(JsonStream::ContextType::Array, "possible_ancestors");
                    stream.Append(possibleAncestor);
                    stream.PopContext();
                }
                
                stream.PopContext();
            }

            response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).Send(stream.Flush());

            handled = true;
        } catch (const rs::scriptobject::ScriptObjectException&) {
            throw InvalidJson{};
        }
    }
    
    return handled;
}

bool RestServer::PostEnsureFullCommit(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool handled = false;
    auto db = GetDatabase(args);
    if (!!db) {
        JsonStream stream;
        
        stream.Append("instance_start_time", db->InstanceStartTime());
        stream.Append("ok", true);
        
        response->setStatusCode(201).setContentType(ContentTypes::applicationJson).Send(stream.Flush());
        
        handled = true;
    }
    
    return handled;
}

bool RestServer::GetLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool gotDoc = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        
        auto doc = db->GetLocalDocument(id);
        auto obj = doc->getObject();
        
        auto rev = doc->getRev();

        auto& stream = response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).getResponseStream();
        ScriptObjectResponseStream<> objStream{stream};
        objStream << obj;
        objStream.Flush();
        
        gotDoc = true;
    }
    
    return gotDoc;
}

bool RestServer::PutLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        auto obj = GetRequestBody(request);        

        if (!!obj) {
            auto doc = db->SetLocalDocument(id, obj);
            
            auto rev = doc->getRev();
            
            JsonStream stream;
            stream.Append("ok", true);
            stream.Append("id", doc->getId());
            stream.Append("rev", rev);

            response->setStatusCode(201).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).Send(stream.Flush());

            created = true;
        }
    }
    return created;
}

bool RestServer::DeleteDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool deleted = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        auto oldRev = GetParameter("rev", request->getQueryString()).c_str();
        
        db->DeleteDocument(id, oldRev);
        
        DocumentRevision::RevString newRev;
        DocumentRevision::Parse(oldRev).Increment().FormatRevision(newRev);
            
        JsonStream stream;
        stream.Append("ok", true);
        stream.Append("id", id);
        stream.Append("rev", newRev.data());

        response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).setETag(newRev.data()).Send(stream.Flush());
        
        deleted = true;        
    }
    
    return deleted;
}

bool RestServer::DeleteDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool deleted = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("designid", args);
        auto oldRev = GetParameter("rev", request->getQueryString()).c_str();
        
        db->DeleteDesignDocument(id, oldRev);
        
        DocumentRevision::RevString newRev;
        DocumentRevision::Parse(oldRev).Increment().FormatRevision(newRev);
            
        JsonStream stream;
        stream.Append("ok", true);
        stream.Append("id", id);
        stream.Append("rev", newRev.data());

        response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).setETag(newRev.data()).Send(stream.Flush());
        
        deleted = true;        
    }
    
    return deleted;
}

bool RestServer::DeleteLocalDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool deleted = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        auto oldRev = GetParameter("rev", request->getQueryString()).c_str();
        
        db->DeleteLocalDocument(id, oldRev);
        
        DocumentRevision::RevString newRev;
        DocumentRevision::Parse(oldRev).Increment().FormatRevision(newRev);
            
        JsonStream stream;
        stream.Append("ok", true);
        stream.Append("id", id);
        stream.Append("rev", newRev.data());

        response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).setETag(newRev.data()).Send(stream.Flush());
        
        deleted = true;        
    }
    
    return deleted;
}

bool RestServer::HeadDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool gotHead = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        
        auto doc = db->GetDocument(id);
        auto rev = doc->getRev();
        
        response->setStatusCode(200).setContentType(ContentTypes::Utf8::applicationJson).setETag(rev).Send();
        
        gotHead = true;
    }
    
    return gotHead;
}

bool RestServer::HeadDesignDocument(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool gotHead = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("designid", args);
        
        auto doc = db->GetDesignDocument(id);
        auto rev = doc->getRev();
        
        response->setStatusCode(200).setContentType(ContentTypes::applicationJson).setETag(rev).Send();
        
        gotHead = true;
    }
    
    return gotHead;
}

bool RestServer::GetDatabaseAllDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto db = GetDatabase(args);
    if (!!db) {
        GetAllDocumentsOptions options(request->getQueryString());
        
        const auto includeDocs = options.IncludeDocs();
        const auto updateSequence = options.UpdateSequence();
        
        DocumentCollection::size_type offset = 0, totalDocs = 0;
        sequence_type updateSequenceNumber = 0;
        auto docs = db->GetDocuments(options, offset, totalDocs, updateSequenceNumber);
        
        auto& stream = response->setContentType(ContentTypes::Utf8::applicationJson).getResponseStream();
        ScriptObjectResponseStream<> objStream{stream};
        objStream << R"({"offset":)" << offset << R"(,"total_rows":)" << totalDocs;
        
        if (updateSequence) {
            objStream << R"(,"update_seq":)" << updateSequenceNumber;
        }
        
        objStream << R"(,"rows":[)";
        
        for (decltype(docs->size()) i = 0, size = docs->size(); i < size; ++i) {
            if (i > 0) {
                objStream << ',';
            }

            auto doc = (*docs)[i];

            auto id = doc->getId();
            auto rev= doc->getRev();

            objStream << R"({"id":")" << id << R"(",)";
            objStream << R"("key":")" << id << R"(",)";
            objStream << R"("value":{"rev":")" << rev << R"("})";

            if (includeDocs) {
                objStream << R"(,"doc":)" << doc->getObject();
            }

            objStream << '}';
        }
        
        objStream << "]}";
        objStream.Flush();        
    }
}

bool RestServer::PostDatabaseAllDocs(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto db = GetDatabase(args);
    if (!!db) {
        auto obj = GetRequestBody(request);
        if (!obj || obj->getType("keys") != rs::scriptobject::ScriptObjectType::Array) {
            throw InvalidJson();
        }
        
        auto keys = obj->getArray("keys");
        
        PostAllDocumentsOptions options(request->getQueryString(), keys);
        
        const auto includeDocs = options.IncludeDocs();
        const auto updateSequence = options.UpdateSequence();
        
        DocumentCollection::size_type totalDocs = 0;
        sequence_type updateSequenceNumber = 0;
        auto docs = db->PostDocuments(options, totalDocs, updateSequenceNumber);
        
        auto& stream = response->setContentType(ContentTypes::Utf8::applicationJson).getResponseStream();
        ScriptObjectResponseStream<> objStream{stream};
        objStream << R"({"offset":0,"total_rows":)" << totalDocs;
        
        if (updateSequence) {
            objStream << R"(,"update_seq":)" << updateSequenceNumber;
        }
        
        objStream << R"(,"rows":[)";
        
        for (decltype(docs->size()) i = 0, size = docs->size(); i < size; ++i) {
            if (i > 0) {
                objStream << ',';
            }

            auto doc = (*docs)[i];

            if (!!doc) {
                auto id = doc->getId();
                auto rev= doc->getRev();

                objStream << R"({"id":")" << id << R"(",)";
                objStream << R"("key":")" << id << R"(",)";
                objStream << R"("value":{"rev":")" << rev << R"("})";

                if (includeDocs) {
                    objStream << R"(,"doc":)" << doc->getObject();
                }

                objStream << '}';
            } else {
                objStream << R"({"key":)";
                objStream.Serialize(keys, i);
                objStream << R"(,"error":"not_found"})";
            }
        }
        
        objStream << "]}";
        objStream.Flush();
    }
}

bool RestServer::GetDatabaseRevsLimit(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto db = GetDatabase(args);
    if (!!db) {
        response->setStatusCode(200).setContentType(ContentTypes::applicationJson).Send("1");
    }
}

bool RestServer::PostTempView(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    auto executed = false;
    
    auto db = GetDatabase(args);
    if (!!db) {
        GetViewOptions options{request->getQueryString()};
        const auto includeDocs = options.IncludeDocs();
        
        auto obj = GetRequestBody(request);
        if (!obj || obj->getType("map") != rs::scriptobject::ScriptObjectType::String) {
            throw InvalidJson();
        }
        
        auto results = db->PostTempView(options, obj);        
        
        auto& stream = response->setContentType(ContentTypes::Utf8::applicationJson).getResponseStream();
        ScriptObjectResponseStream<> objStream{stream};
        objStream << R"({"offset":)" << results->Offset() << R"(,"total_rows":)" << results->TotalRows() << R"(,"rows":[)";

        auto prefixComma = false;
        auto iter = results->Iterator();
        auto result = iter.Next();
        while (result) {
            auto resultObj = result->getResultArray();
            
            objStream << (prefixComma ? ',' : ' ');
            objStream << R"({"id":")" << result->getId() << R"(","key":)";
            objStream.Serialize(resultObj, MapReduceResult::KeyIndex);
            objStream << R"(,"value":)";
            objStream.Serialize(resultObj, MapReduceResult::ValueIndex);
            
            if (includeDocs) {
                objStream << R"(,"doc":)" << result->getDoc()->getObject();
            }
            
            objStream << '}';
            prefixComma = true;
            
            result = iter.Next();
        }
        
        objStream << "]}";
        objStream.Flush();
        
        executed = true;
    }
    
    return executed;
}

bool RestServer::GetConfig(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType(ContentTypes::applicationJson).Send(RestConfig::getConfig());
    return true;
}

bool RestServer::GetConfigQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType(ContentTypes::applicationJson).Send(RestConfig::getQueryServers());
    return true;
}

bool RestServer::GetConfigNativeQueryServers(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs&, rs::httpserver::response_ptr response) {
    response->setContentType(ContentTypes::applicationJson).Send(RestConfig::getNativeQueryServers());
    return true;
}

bool RestServer::PutDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool created = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);
        auto oldRev = GetParameter("rev", request->getQueryString());
        auto oldDoc = db->GetDocument(id, true);
        if (std::strcmp(oldRev.c_str(), oldDoc->getRev()) != 0) {
            throw DocumentConflict{};
        }

        auto name = GetParameter("attname", args);
        auto contentType = request->getContentType();
        auto contentLength = request->getContentLength();
        auto& requestStream = request->getRequestStream();

        std::vector<rs::httpserver::RequestStream::byte> buffer(contentLength);

        decltype(contentLength) offset = 0;
        while (offset < contentLength) {
            auto remaining = contentLength - offset;
            auto bytesRead = requestStream.Read(&buffer[offset], 0, remaining, false);

            if (bytesRead <= 0) {
                throw BadRequestBodyError{};
            }

            offset += bytesRead;
        }
        
        auto newDoc = db->SetDocumentAttachment(id, oldRev.c_str(), name, contentType.c_str(), buffer);
        auto newRev = newDoc->getRev();

        JsonStream stream;
        stream.Append("ok", true);
        stream.Append("id", id);
        stream.Append("rev", newRev);

        response->setStatusCode(201).setContentType(ContentTypes::applicationJson).setETag(newRev).Send(stream.Flush());

        created = true;
    }
    
    return created;
}

bool RestServer::HeadDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool found = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);        
        auto name = GetParameter("attname", args);
        
        auto attachment = db->GetDocumentAttachment(id, name, false);

        auto& contentType = attachment->ContentType();
        auto& digest = attachment->Digest();       
        auto size = attachment->Size();
        
        response->setContentType(contentType).setETag(digest).setContentLength(size).Send();
        
        found = true;
    }
    
    return found;
}

bool RestServer::GetDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool found = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);        
        auto name = GetParameter("attname", args);
        
        auto attachment = db->GetDocumentAttachment(id, name, true);

        auto& contentType = attachment->ContentType();
        auto& digest = attachment->Digest();
        
        auto requestedRange = request->getRange();
        auto ifNoneMatch = request->getIfNoneMatch();
        
        if (requestedRange.size() == 0 && ifNoneMatch.size() > 0 && ifNoneMatch == digest) {
            response->setStatusCode(304).setContentType(contentType, false).setETag(digest).Send();
        } else {
            auto size = attachment->Size();
            auto contentTypeInfo = rs::httpserver::MimeTypes::GetContentType(contentType);
            auto compress = contentTypeInfo.is_initialized() && contentTypeInfo.get().getCompressible();
            
            auto range = request->getByteRanges();
            if (contentType == ContentTypes::applicationOctetStream && range.size() == 1) {
                auto rangeStart = range[0].first;
                auto rangeEnd = range[0].second;
                
                if (rangeStart < 0) {
                    rangeStart += size;
                }
                
                if (rangeStart < 0 || rangeStart >= size) {
                    throw BadRangeError{};
                }
                
                auto endIndex = static_cast<decltype(rangeEnd)>(size > 0 ? size - 1 : 0);
                rangeEnd = std::min(rangeEnd, endIndex);
                auto rangeSize = std::min(rangeEnd - rangeStart + 1, endIndex);
                
                auto contentRange = (boost::format("bytes %1%-%2%/%3%") % rangeStart % rangeEnd % rangeSize).str();
                
                response->setStatusCode(206).setStatusDescription("Partial Content").setContentType(contentType, compress).setContentRange(contentRange);
                auto& stream = response->getResponseStream();
                stream.Write(attachment->Data(), rangeStart, rangeSize);
                stream.Flush();
            } else {
                auto& stream = response->setContentType(contentType, compress).setETag(digest).getResponseStream();
                stream.Write(attachment->Data(), 0, size);
                stream.Flush();
            }

            found = true;
        }
    }
    
    return found;
}

bool RestServer::DeleteDocumentAttachment(rs::httpserver::request_ptr request, const rs::httpserver::RequestRouter::CallbackArgs& args, rs::httpserver::response_ptr response) {
    bool deleted = false;
    auto db = GetDatabase(args);
    if (!!db) {
        auto id = GetParameter("id", args);        
        auto oldRev = GetParameter("rev", request->getQueryString());
        auto name = GetParameter("attname", args);
        
        auto newDoc = db->DeleteDocumentAttachment(id, oldRev.c_str(), name);
        auto newRev = newDoc->getRev();

        JsonStream stream;
        stream.Append("ok", true);
        stream.Append("id", id);
        stream.Append("rev", newRev);

        response->setStatusCode(200).setContentType(ContentTypes::applicationJson).setETag(newRev).Send(stream.Flush());
        
        deleted = true;
    }
    
    return deleted;
}

database_ptr RestServer::GetDatabase(const rs::httpserver::RequestRouter::CallbackArgs& args) {
    database_ptr db;
    auto dbName = GetDatabaseName(args);
    if (dbName != nullptr) {                
        db = databases_.GetDatabase(dbName);
    }
    return db;
}

const char* RestServer::GetDatabaseName(const rs::httpserver::RequestRouter::CallbackArgs& args) {
    const char* dbName = nullptr;
    auto argsIter = args.find("db");
    if (argsIter != args.cend()) {
        dbName = argsIter->second;
    }
    return dbName;
}

const char* RestServer::GetParameter(const char* param, const rs::httpserver::RequestRouter::CallbackArgs& args) {
    auto argsIter = args.find(param);
    if (argsIter == args.cend()) {
        throw InvalidJson();
    }
    
    return argsIter->second;
}

const std::string& RestServer::GetParameter(const char* param, const rs::httpserver::QueryString& qs, bool throwIfMissing) {
    if (throwIfMissing && !qs.IsKey(param)) {
        // TODO: this isn't the best exception message to return, however there are no obvious alternatives that CouchDB uses
        throw InvalidJson();
    }
    
    return qs.getValue(param);
}

rs::scriptobject::ScriptObjectPtr RestServer::GetRequestBody(rs::httpserver::request_ptr request, bool useCachedObjectKeys) {
    if (request->HasBody()) {
        bool gotJson = request->getContentType().find(ContentTypes::applicationJson) != std::string::npos;
        bool gotMsgPack = !gotJson && request->getContentType().find(ContentTypes::applicationMsgPack) != std::string::npos;
        
        if (!gotJson && !gotMsgPack) {
            throw InvalidJson{};
        }

        auto& requestStream = request->getRequestStream();
        auto requestLength = request->getContentLength();
        
        std::vector<rs::httpserver::RequestStream::byte> buffer(requestLength);
        
        decltype(requestLength) offset = 0;
        while (offset < requestLength) {
            auto remaining = requestLength - offset;
            auto bytesRead = requestStream.Read(&buffer[offset], 0, remaining, false);
            
            if (bytesRead <= 0) {
                throw InvalidJson{};
            }
            
            offset += bytesRead;
        }
        
        auto json = reinterpret_cast<char*>(buffer.data());
        
        if (gotJson) {
            try {   
                rs::scriptobject::ScriptObjectJsonSource source(json);
                return rs::scriptobject::ScriptObjectFactory::CreateObject(source, useCachedObjectKeys);
            } catch (const std::exception&) {
                throw InvalidJson{};
            }
        } else {
            try {
                rs::scriptobject::ScriptObjectMsgpackSource source(json, requestLength);
                return rs::scriptobject::ScriptObjectFactory::CreateObject(source, useCachedObjectKeys);
            } catch (const std::exception&) {
                throw InvalidMsgPack{};
            }
        }
    } else {
        return rs::scriptobject::ScriptObjectPtr{};
    }
}
