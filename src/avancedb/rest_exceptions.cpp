#include "rest_exceptions.h"
#include "database.h"

static const char* preconditionFailedDescription = "Precondition Failed";
static const char* badRequestDescription = "Bad Request";
static const char* notFoundDescription = "Not Found";
static const char* conflictDescription = "Conflict";

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

DocumentConflict::DocumentConflict() :
    HttpServerException(409, conflictDescription, documentConflictJsonBody, contentType) {
    
}