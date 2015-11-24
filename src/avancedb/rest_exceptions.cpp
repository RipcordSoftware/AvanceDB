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

#include "rest_exceptions.h"

#include <boost/format.hpp>

#include "json_helper.h"

static const char* preconditionFailedDescription = "Precondition Failed";
static const char* badRequestDescription = "Bad Request";
static const char* notFoundDescription = "Not Found";
static const char* conflictDescription = "Conflict";
static const char* forbiddenDescription = "Forbidden";
static const char* requestedRangeErrorDescription = "Requested Range Not Satisfiable";
static const char* internalServerErrorDescription = "Internal Server Error";

static const char* databaseAlreadyExistsBody = R"({
    "error": "file_exists",
    "reason": "The database could not be created, the file already exists."
})";

static const char* invalidDatabaseNameBody = R"({
    "error": "illegal_database_name",
    "reason": "Only lowercase characters (a-z), digits (0-9), and any of the characters _, $, (, ), +, -, and / are allowed. Must begin with a letter."
})";

static const char* missingDatabaseBody = R"({
    "error": "not_found",
    "reason": "missing"
})";

static const char* invalidJsonBody = R"({
    "error":"bad_request",
    "reason":"invalid_json"
})";

static const char* documentConflictJsonBody = R"({
    "error":"conflict",
    "reason":"Document update conflict."
})";

static const char* missingDocumentJsonBody = R"({
    "error": "not_found",
    "reason": "missing"
})";

static const char* missingDocumentAttachmentJsonBody = R"({
    "error": "not_found",
    "reason": "Document is missing attachment"
})";

static const char* uuidCountLimitJsonBody = R"({
    "error": "forbidden",
    "reason": "count parameter too large"
})";

static const char* queryParseErrorJsonBody = R"({
    "error": "query_parse_error",
    "reason": "Invalid value for %s: \"%s\""
})";

static const char* invalidRevisionFormatJsonBody = R"({
    "error": "bad_request",
    "reason": "Invalid rev format"
})";

static const char* compilationErrorJsonBody = R"({
    "error": "compilation_error",
    "reason": "%s"
})";

static const char* badLanguageErrorJsonBody = R"({
    "error": "EXIT",
    "reason": "%s is not a supported map/reduce language"
})";

static const char* requestedRangeErrorJsonBody = R"({
    "error": "requested_range_not_satisfiable",
    "reason": "Requested range not satisfiable"
})";

static const char* contentType = "application/json";

DatabaseAlreadyExists::DatabaseAlreadyExists() : 
    HttpServerException(412, preconditionFailedDescription, databaseAlreadyExistsBody, contentType) {
    
}

InvalidDatabaseName::InvalidDatabaseName() :
    HttpServerException(400, badRequestDescription, invalidDatabaseNameBody, contentType) {
    
}

MissingDatabase::MissingDatabase() :
    HttpServerException(404, notFoundDescription, missingDatabaseBody, contentType) {
    
}

InvalidJson::InvalidJson() :
    HttpServerException(400, badRequestDescription, invalidJsonBody, contentType) {
    
}

InvalidMsgPack::InvalidMsgPack() :
    HttpServerException(400, badRequestDescription, invalidJsonBody, contentType) {
    
}

DocumentConflict::DocumentConflict() :
    HttpServerException(409, conflictDescription, documentConflictJsonBody, contentType) {
    
}

DocumentMissing::DocumentMissing() :
    HttpServerException(404, notFoundDescription, missingDocumentJsonBody, contentType) {
    
}

DocumentAttachmentMissing::DocumentAttachmentMissing() :
    HttpServerException(404, notFoundDescription, missingDocumentAttachmentJsonBody, contentType) {
    
}

UuidCountLimit::UuidCountLimit() :
    HttpServerException(403, forbiddenDescription, uuidCountLimitJsonBody, contentType) {
    
}

QueryParseError::QueryParseError(const char* type, const std::string& value) :
    HttpServerException(400, badRequestDescription, (boost::format(queryParseErrorJsonBody) % type % value).str(), contentType) {
    
}

InvalidRevisionFormat::InvalidRevisionFormat() :
    HttpServerException(400, badRequestDescription, invalidRevisionFormatJsonBody, contentType) {
    
}

CompilationError::CompilationError(const char* msg) :
    HttpServerException(500, internalServerErrorDescription, (boost::format(compilationErrorJsonBody) % JsonHelper::EscapeJsonString(msg)).str(), contentType) {
    
}

BadLanguageError::BadLanguageError(const char* msg) :
    HttpServerException(500, internalServerErrorDescription, (boost::format(badLanguageErrorJsonBody) % JsonHelper::EscapeJsonString(msg)).str(), contentType) {
    
}

BadRangeError::BadRangeError() :
    HttpServerException(416, requestedRangeErrorDescription, requestedRangeErrorJsonBody, contentType) {
    
}

BadRequestBodyError::BadRequestBodyError() :
    HttpServerException(400, badRequestDescription, std::string{}, contentType) {
    
}