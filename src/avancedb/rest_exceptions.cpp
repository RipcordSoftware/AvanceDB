#include "rest_exceptions.h"
#include "database.h"

static const char* preconditionFailedDescription = "Precondition Failed";
static const char* badRequestDescription = "Bad Request";

static const char* databaseAlreadyExistsBody = R"({
    "error": "file_exists",
    "reason": "The database could not be created, the file already exists."
})";

static const char* invalidDatabaseNameBody = R"({
    "error": "illegal_database_name",
    "reason": "Only lowercase characters (a-z), digits (0-9), and any of the characters _, $, (, ), +, -, and / are allowed. Must begin with a letter."
})";

static const char* contentType = "application/json";

DatabaseAlreadyExists::DatabaseAlreadyExists() : 
    HttpServerException(412, preconditionFailedDescription, databaseAlreadyExistsBody, contentType) {
    
}

InvalidDatabaseName::InvalidDatabaseName() :
    HttpServerException(400, badRequestDescription, invalidDatabaseNameBody, contentType) {
    
}