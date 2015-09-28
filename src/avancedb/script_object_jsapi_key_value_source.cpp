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

#include "script_object_jsapi_key_value_source.h"

#include "script_object_jsapi_source.h"
#include "script_array_jsapi_source.h"

#include "script_object_factory.h"
#include "script_array_factory.h"

ScriptObjectJsapiKeyValueSource ScriptObjectJsapiKeyValueSource::Create(const rs::jsapi::Value& key, const rs::jsapi::Value& value) {
    ScriptObjectJsapiKeyValueSource source;
    
    auto cx = key.getContext();
    JSAutoRequest ar{cx};        
    
    source.values_ = std::vector<rs::jsapi::Value>{ key, value };
    
    source.stringValues_.resize(source.values_.size());

    for (decltype(source.values_.size()) i = 0, length = source.values_.size(); i < length; ++i) {
        switch (JS_TypeOfValue(cx, source.values_[i])) {
            case JSTYPE_OBJECT: 
                if (source.values_[i].isArray()) {
                    source.types_.push_back(ScriptObjectType::Array);
                } else {
                    source.types_.push_back(ScriptObjectType::Object);
                }
                break;

            case JSTYPE_STRING: 
                source.types_.push_back(ScriptObjectType::String); 
                source.stringValues_[i] = source.values_[i].ToString(); 
                break;

            case JSTYPE_NUMBER: source.types_.push_back(ScriptObjectType::Double); break;
            case JSTYPE_BOOLEAN: source.types_.push_back(ScriptObjectType::Boolean); break;
            case JSTYPE_NULL: source.types_.push_back(ScriptObjectType::Null); break;
            default: source.types_.push_back(ScriptObjectType::Undefined); break;
        }
    }
    
    return source;
}

ScriptObjectJsapiKeyValueSource::ScriptObjectJsapiKeyValueSource() {
    
}

ScriptObjectJsapiKeyValueSource::ScriptObjectJsapiKeyValueSource(ScriptObjectJsapiKeyValueSource&& rhs) :
        values_(std::move(rhs.values_)),
        types_(std::move(rhs.types_)), stringValues_(std::move(stringValues_)) {
    
}

ScriptObjectJsapiKeyValueSource& ScriptObjectJsapiKeyValueSource::operator=(ScriptObjectJsapiKeyValueSource&& rhs) {
    values_ = std::move(rhs.values_);
    stringValues_ = std::move(rhs.stringValues_);
    types_ = std::move(rhs.types_);
    return *this;
}

unsigned ScriptObjectJsapiKeyValueSource::count() const {
    return values_.size();
}

rs::scriptobject::ScriptObjectType ScriptObjectJsapiKeyValueSource::type(int index) const { 
    return types_[index];
}

const char* ScriptObjectJsapiKeyValueSource::name(int index) const {
    switch (index) {
        case 0: return "key";
        case 1: return "value";
        default: return nullptr;
    }
}

unsigned ScriptObjectJsapiKeyValueSource::length(int index) const {
    switch (index) {
        case 0: return sizeof("key") - 1;
        case 1: return sizeof("value") - 1;
        default: return 0;
    }
}

bool ScriptObjectJsapiKeyValueSource::getBoolean(int index) const {
    return values_[index].toBoolean();
}

std::int32_t ScriptObjectJsapiKeyValueSource::getInt32(int index) const {
    return values_[index].toInt32();
}

double ScriptObjectJsapiKeyValueSource::getDouble(int index) const {
    return values_[index].toNumber();
}

const char* ScriptObjectJsapiKeyValueSource::getString(int index) const {
    return stringValues_[index].c_str();
}

int ScriptObjectJsapiKeyValueSource::getStringLength(int index) const {
    return stringValues_[index].length();
}

const rs::scriptobject::ScriptObjectPtr ScriptObjectJsapiKeyValueSource::getObject(int index) const {
    auto source = ScriptObjectJsapiSource::Create(values_[index]);
    return rs::scriptobject::ScriptObjectFactory::CreateObject(source);
}

const rs::scriptobject::ScriptArrayPtr ScriptObjectJsapiKeyValueSource::getArray(int index) const {
    auto source = ScriptArrayJsapiSource::Create(values_[index]);
    return rs::scriptobject::ScriptArrayFactory::CreateArray(source);
}