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

#ifndef SCRIPT_OBJECT_JSAPI_EXCEPTIONS_H
#define SCRIPT_OBJECT_JSAPI_EXCEPTIONS_H

#include <string>
#include <exception>

class ScriptObjectJsapiSourceException : public std::exception {
public:
    ScriptObjectJsapiSourceException(const char* what) : what_(what) {}
    
    virtual const char* what() const noexcept override { return what_.c_str(); }
    
private:
    const std::string what_;
};

class ScriptObjectJsapiSourceInvalidFieldTypeException final : public ScriptObjectJsapiSourceException {
public:
    ScriptObjectJsapiSourceInvalidFieldTypeException() : ScriptObjectJsapiSourceException("Invalid field type") {}
};

#endif	/* SCRIPT_OBJECT_JSAPI_EXCEPTIONS_H */
