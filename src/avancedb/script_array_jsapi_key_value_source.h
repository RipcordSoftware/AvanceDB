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

#ifndef RS_AVANCEDB_SCRIPT_OBJECT_KEY_VALUE_SOURCE_H
#define RS_AVANCEDB_SCRIPT_OBJECT_KEY_VALUE_SOURCE_H

#include <array>

#include "script_array_source.h"

#include "libjsapi.h"

class ScriptArrayJsapiKeyValueSource final : public rs::scriptobject::ScriptArraySource  {
    using ScriptObjectType = rs::scriptobject::ScriptObjectType;
    
public:
    
    ScriptArrayJsapiKeyValueSource(ScriptArrayJsapiKeyValueSource&& rhs);
    ScriptArrayJsapiKeyValueSource& operator=(ScriptArrayJsapiKeyValueSource&& rhs);
    
    static ScriptArrayJsapiKeyValueSource Create(const rs::jsapi::Value& key, const rs::jsapi::Value& value);        
    
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
    
    ScriptArrayJsapiKeyValueSource(const rs::jsapi::Value& key, const rs::jsapi::Value& value);    
    
    std::array<rs::jsapi::Value, 2> values_;
    std::array<std::string, 2> stringValues_;
    std::array<ScriptObjectType, 2> types_;

};

#endif /* RS_AVANCEDB_SCRIPT_OBJECT_KEY_VALUE_SOURCE_H */

