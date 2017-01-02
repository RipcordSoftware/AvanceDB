/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2017 Ripcord Software
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

#ifndef RS_AVANCEDB_SCRIPT_ARRAY_JSAPI_SOURCE_H
#define RS_AVANCEDB_SCRIPT_ARRAY_JSAPI_SOURCE_H

#include <vector>
#include <string>

#include "script_array_source.h"

#include "libjsapi.h"

class ScriptArrayJsapiSource final : public rs::scriptobject::ScriptArraySource  {
    using ScriptObjectType = rs::scriptobject::ScriptObjectType;
    
public:
    
    ScriptArrayJsapiSource(ScriptArrayJsapiSource&& rhs);
    ScriptArrayJsapiSource& operator=(ScriptArrayJsapiSource&& rhs);
    
    static ScriptArrayJsapiSource Create(const rs::jsapi::Value&);
    
    virtual unsigned count() const override;
    virtual ScriptObjectType type(int index) const override;
    virtual bool getBoolean(int index) const override;
    virtual std::int32_t getInt32(int index) const override;
    virtual std::uint32_t getUInt32(int index) const override;
    virtual std::int64_t getInt64(int index) const override;
    virtual std::uint64_t getUInt64(int index) const override;
    virtual double getDouble(int index) const override;
    virtual const char* getString(int index) const override;
    virtual int getStringLength(int index) const override;
    virtual const rs::scriptobject::ScriptObjectPtr getObject(int index) const override;
    virtual const rs::scriptobject::ScriptArrayPtr getArray(int index) const override;
    
private:
    
    ScriptArrayJsapiSource();

    std::vector<rs::jsapi::Value> values_;
    std::vector<std::string> stringValues_;
    std::vector<ScriptObjectType> types_;
    
};

#endif /* RS_AVANCEDB_SCRIPT_ARRAY_JSAPI_SOURCE_H */