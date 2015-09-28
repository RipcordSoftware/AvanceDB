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

#include "script_object_jsapi_source.h"
#include "script_array_jsapi_source.h"

#include "script_object_factory.h"
#include "script_array_factory.h"

#include "jsapi.h"

ScriptObjectJsapiSource ScriptObjectJsapiSource::Create(const rs::jsapi::Value& obj) {
    ScriptObjectJsapiSource source;
    
    auto cx = obj.getContext();
    JSAutoRequest ar{cx};    
    JS::AutoIdArray ids{cx, JS_Enumerate(cx, obj)};
    
    if (!!ids) {        
        source.stringValues_.resize(ids.length());
        
        for (decltype(ids.length()) i = 0, length = ids.length(); i < length; ++i) {
            JS::RootedId id{cx, ids[i]};
            
            rs::jsapi::Value key{cx};
            if (JS_IdToValue(cx, id, key)) {
                source.keys_.emplace_back(key.ToString());
                
                JS::RootedValue value{cx};
                if (JS_GetPropertyById(cx, obj, id, &value)) {
                    source.values_.emplace_back(cx, value);
                } else {
                    source.values_.emplace_back(cx);
                }                                
                
                switch (JS_TypeOfValue(cx, value)) {
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
        }
    }
    
    return source;
}

ScriptObjectJsapiSource::ScriptObjectJsapiSource() {
    
}

ScriptObjectJsapiSource::ScriptObjectJsapiSource(ScriptObjectJsapiSource&& rhs) :
        keys_(std::move(rhs.keys_)), values_(std::move(rhs.values_)), 
        types_(std::move(rhs.types_)), stringValues_(std::move(stringValues_)) {
    
}

ScriptObjectJsapiSource& ScriptObjectJsapiSource::operator=(ScriptObjectJsapiSource&& rhs) {
    keys_ = std::move(rhs.keys_);
    values_ = std::move(rhs.values_);
    stringValues_ = std::move(rhs.stringValues_);
    types_ = std::move(rhs.types_);
    return *this;
}

unsigned ScriptObjectJsapiSource::count() const {    
    return keys_.size();
}

unsigned ScriptObjectJsapiSource::length(int index) const {
    return keys_[index].length();
}

rs::scriptobject::ScriptObjectType ScriptObjectJsapiSource::type(int index) const { 
    return types_[index];
}

const char* ScriptObjectJsapiSource::name(int index) const {
    return keys_[index].c_str();
}

bool ScriptObjectJsapiSource::getBoolean(int index) const {
    return values_[index].toBoolean();
}

std::int32_t ScriptObjectJsapiSource::getInt32(int index) const {
    return values_[index].toInt32();
}

double ScriptObjectJsapiSource::getDouble(int index) const {
    return values_[index].toNumber();
}

const char* ScriptObjectJsapiSource::getString(int index) const {
    return stringValues_[index].c_str();
}

int ScriptObjectJsapiSource::getStringLength(int index) const {
    return stringValues_[index].length();
}

const rs::scriptobject::ScriptObjectPtr ScriptObjectJsapiSource::getObject(int index) const {
    auto source = Create(values_[index]);
    return rs::scriptobject::ScriptObjectFactory::CreateObject(source);
}

const rs::scriptobject::ScriptArrayPtr ScriptObjectJsapiSource::getArray(int index) const {
    auto source = ScriptArrayJsapiSource::Create(values_[index]);
    return rs::scriptobject::ScriptArrayFactory::CreateArray(source);
}