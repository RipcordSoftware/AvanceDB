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

#include "script_array_jsapi_key_value_source.h"

#include "script_object_jsapi_source.h"
#include "script_array_jsapi_source.h"

#include "script_object_factory.h"
#include "script_array_factory.h"

ScriptArrayJsapiKeyValueSource ScriptArrayJsapiKeyValueSource::Create(const rs::jsapi::Value& key, const rs::jsapi::Value& value) {
    ScriptArrayJsapiKeyValueSource source;
    
    auto cx = key.getContext();
    JSAutoRequest ar{cx};        
    
    source.values_ = std::vector<rs::jsapi::Value>{ key, value };
    
    source.stringValues_.resize(source.values_.size());

    for (decltype(source.values_.size()) i = 0, length = source.values_.size(); i < length; ++i) {
        switch (JS_TypeOfValue(cx, source.values_[i])) {
            case JSTYPE_OBJECT: 
                if (source.values_[i].isArray() || rs::jsapi::DynamicArray::IsDynamicArray(source.values_[i])) {
                    source.types_.push_back(ScriptObjectType::Array);
                } else if (!source.values_[i].isNull()) {
                    source.types_.push_back(ScriptObjectType::Object);
                } else {
                    source.types_.push_back(ScriptObjectType::Null);
                }
                break;

            case JSTYPE_STRING: 
                source.types_.push_back(ScriptObjectType::String); 
                source.stringValues_[i] = source.values_[i].ToString(); 
                break;

            case JSTYPE_NUMBER: source.types_.push_back(ScriptObjectType::Double); break;
            case JSTYPE_BOOLEAN: source.types_.push_back(ScriptObjectType::Boolean); break;
            default: source.types_.push_back(ScriptObjectType::Null); break;
        }
    }
    
    return source;
}

ScriptArrayJsapiKeyValueSource::ScriptArrayJsapiKeyValueSource() {
    
}

ScriptArrayJsapiKeyValueSource::ScriptArrayJsapiKeyValueSource(ScriptArrayJsapiKeyValueSource&& rhs) :
        values_(std::move(rhs.values_)),
        types_(std::move(rhs.types_)), stringValues_(std::move(stringValues_)) {
    
}

ScriptArrayJsapiKeyValueSource& ScriptArrayJsapiKeyValueSource::operator=(ScriptArrayJsapiKeyValueSource&& rhs) {
    values_ = std::move(rhs.values_);
    stringValues_ = std::move(rhs.stringValues_);
    types_ = std::move(rhs.types_);
    return *this;
}

unsigned ScriptArrayJsapiKeyValueSource::count() const {
    return values_.size();
}

rs::scriptobject::ScriptObjectType ScriptArrayJsapiKeyValueSource::type(int index) const { 
    return types_[index];
}

bool ScriptArrayJsapiKeyValueSource::getBoolean(int index) const {
    return values_[index].toBoolean();
}

std::int32_t ScriptArrayJsapiKeyValueSource::getInt32(int index) const {
    return values_[index].toInt32();
}

double ScriptArrayJsapiKeyValueSource::getDouble(int index) const {
    return values_[index].toNumber();
}

const char* ScriptArrayJsapiKeyValueSource::getString(int index) const {
    return stringValues_[index].c_str();
}

int ScriptArrayJsapiKeyValueSource::getStringLength(int index) const {
    return stringValues_[index].length();
}

const rs::scriptobject::ScriptObjectPtr ScriptArrayJsapiKeyValueSource::getObject(int index) const {
    auto source = ScriptObjectJsapiSource::Create(values_[index]);
    return rs::scriptobject::ScriptObjectFactory::CreateObject(source);
}

const rs::scriptobject::ScriptArrayPtr ScriptArrayJsapiKeyValueSource::getArray(int index) const {
    auto source = ScriptArrayJsapiSource::Create(values_[index]);
    return rs::scriptobject::ScriptArrayFactory::CreateArray(source);
}