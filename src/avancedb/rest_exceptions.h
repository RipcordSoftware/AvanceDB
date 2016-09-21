/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2016 Ripcord Software
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

#ifndef RS_AVANCEDB_REST_EXCEPTIONS_H
#define RS_AVANCEDB_REST_EXCEPTIONS_H

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

class InvalidMsgPack final : public HttpServerException {
public:
    InvalidMsgPack();
};

class DocumentConflict final : public HttpServerException {
public:
    DocumentConflict();
};

class DocumentMissing final : public HttpServerException {
public:
    DocumentMissing();
};

class DocumentAttachmentMissing final : public HttpServerException {
public:
    DocumentAttachmentMissing();
};

class UuidCountLimit final : public HttpServerException {
public:
    UuidCountLimit();
};

class QueryParseError final : public HttpServerException {
public:
    QueryParseError(const char* type, const std::string& value);    
};

class InvalidRevisionFormat final : public HttpServerException {
public:
    InvalidRevisionFormat();    
};

class CompilationError final : public HttpServerException {
public:
    CompilationError(const char* msg);
};

class BadLanguageError final : public HttpServerException {
public:
    BadLanguageError(const char* msg);
};

class BadRangeError final : public HttpServerException {
public:
    BadRangeError();
};

class BadRequestBodyError final : public HttpServerException {
public:
    BadRequestBodyError();
};

#endif /* RS_AVANCEDB_REST_EXCEPTIONS_H */
