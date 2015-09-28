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

#ifndef SCRIPT_OBJECT_JSAPI_SOURCE_H
#define SCRIPT_OBJECT_JSAPI_SOURCE_H

#include <vector>
#include <string>

#include "script_object_source.h"

#include "libjsapi.h"

class ScriptObjectJsapiSource final : public rs::scriptobject::ScriptObjectSource  {
    using ScriptObjectType = rs::scriptobject::ScriptObjectType;
    
public:
        
    ScriptObjectJsapiSource(ScriptObjectJsapiSource&& rhs);
    ScriptObjectJsapiSource& operator=(ScriptObjectJsapiSource&& rhs);
    
    static ScriptObjectJsapiSource Create(const rs::jsapi::Value&);
    
    virtual unsigned count() const override;
    virtual ScriptObjectType type(int index) const override;
    virtual const char* name(int index) const override;    
    virtual unsigned length(int index) const override;
    
    virtual bool getBoolean(int index) const override;
    virtual std::int32_t getInt32(int index) const override;
    virtual double getDouble(int index) const override;
    virtual const char* getString(int index) const override;
    virtual int getStringLength(int index) const override;
    virtual const rs::scriptobject::ScriptObjectPtr getObject(int index) const override;
    virtual const rs::scriptobject::ScriptArrayPtr getArray(int index) const override;
    
private:
            
    ScriptObjectJsapiSource();
    
    std::vector<std::string> keys_;
    std::vector<rs::jsapi::Value> values_;
    std::vector<std::string> stringValues_;
    std::vector<ScriptObjectType> types_;

};

#endif	/* SCRIPT_OBJECT_JSAPI_SOURCE_H */
