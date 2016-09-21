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

#ifndef HTTP_SERVER_EXCEPTION_H
#define HTTP_SERVER_EXCEPTION_H

#include <exception>
#include <string>

class HttpServerException : public std::exception {
public:
    
    HttpServerException(unsigned statusCode, const char* description, const char* body = nullptr, const char* contentType = nullptr) : 
        statusCode_(statusCode), description_(description), body_(body), contentType_(contentType) {        
    }
        
    HttpServerException(unsigned statusCode, const char* description, const std::string& body = "", const char* contentType = nullptr) : 
        HttpServerException(statusCode, description, body.c_str(), contentType) {
    }
    
    virtual const char* what() const noexcept { return Description(); }
    
    unsigned StatusCode() const noexcept { return statusCode_; }
    const char* Description() const noexcept { return description_.c_str(); }
    const char* Body() const noexcept { return body_.c_str(); }
    const char* ContentType() const noexcept { return contentType_.c_str(); }
    
private:
    const unsigned statusCode_;
    const std::string description_;
    const std::string body_;
    const std::string contentType_;
        
};

#endif	/* HTTP_SERVER_EXCEPTION_H */

