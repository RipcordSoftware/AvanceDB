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

#endif	/* REST_EXCEPTIONS_H */

