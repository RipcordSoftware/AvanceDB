#ifndef REST_EXCEPTIONS_H
#define	REST_EXCEPTIONS_H

#include "http_server_exception.h"

class DatabaseAlreadyExists final : public HttpServerException {
public:
    DatabaseAlreadyExists();
};

class InvalidDatabaseName final : public HttpServerException {
public:
    InvalidDatabaseName();
};

class MissingDatabase final : public HttpServerException {
public:
    MissingDatabase();
};

class InvalidJson final : public HttpServerException {
public:
    InvalidJson();
};

class DocumentConflict final : public HttpServerException {
public:
    DocumentConflict();
};

class DocumentMissing final : public HttpServerException {
public:
    DocumentMissing();
};

class UuidCountLimit final : public HttpServerException {
public:
    UuidCountLimit();
};

#endif	/* REST_EXCEPTIONS_H */

