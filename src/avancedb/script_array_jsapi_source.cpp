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

#include "script_array_jsapi_source.h"
#include "map_reduce.h"
#include "script_object_jsapi_exceptions.h"

#include "script_array_factory.h"
#include "script_object_factory.h"

ScriptArrayJsapiSource ScriptArrayJsapiSource::Create(const rs::jsapi::Value& obj) {
    ScriptArrayJsapiSource source;
    
    auto cx = obj.getContext();
    JSAutoRequest ar{cx};    
    
    uint32_t length = 0;
    if (JS_GetArrayLength(cx, obj, &length) && length > 0) {
        source.stringValues_.resize(length);
        
        for (decltype(length) i = 0; i < length; ++i) {
            rs::jsapi::Value value{cx};
            if (JS_GetElement(cx, obj, i, value)) {                
                source.values_.emplace_back(value);
                
                switch (JS_TypeOfValue(cx, value)) {
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
            } else {
                source.values_.emplace_back(cx);
            }
        }
    }
    
    return source;
}

ScriptArrayJsapiSource::ScriptArrayJsapiSource() {
}

ScriptArrayJsapiSource::ScriptArrayJsapiSource(ScriptArrayJsapiSource&& rhs) :
        values_(std::move(rhs.values_)), 
        types_(std::move(rhs.types_)), stringValues_(std::move(rhs.stringValues_)) {    
}

ScriptArrayJsapiSource& ScriptArrayJsapiSource::operator=(ScriptArrayJsapiSource&& rhs) {
    values_ = std::move(rhs.values_);
    stringValues_ = std::move(rhs.stringValues_);
    types_ = std::move(rhs.types_);
    return *this;
}

unsigned ScriptArrayJsapiSource::count() const {
    return values_.size();
}

ScriptArrayJsapiSource::ScriptObjectType ScriptArrayJsapiSource::type(int index) const {
    return types_[index];
}

bool ScriptArrayJsapiSource::getBoolean(int index) const {
    return values_[index].toBoolean();
}

std::int32_t ScriptArrayJsapiSource::getInt32(int index) const {
    return values_[index].toInt32();
}

std::uint32_t ScriptArrayJsapiSource::getUInt32(int index) const {
    throw ScriptObjectJsapiSourceInvalidFieldTypeException{};
}

std::int64_t ScriptArrayJsapiSource::getInt64(int index) const {
    throw ScriptObjectJsapiSourceInvalidFieldTypeException{};
}

std::uint64_t ScriptArrayJsapiSource::getUInt64(int index) const {
    throw ScriptObjectJsapiSourceInvalidFieldTypeException{};
}

double ScriptArrayJsapiSource::getDouble(int index) const {
    return values_[index].toNumber();
}

const char* ScriptArrayJsapiSource::getString(int index) const {
    return stringValues_[index].c_str();
}

int ScriptArrayJsapiSource::getStringLength(int index) const {
    return stringValues_[index].size();
}

const rs::scriptobject::ScriptObjectPtr ScriptArrayJsapiSource::getObject(int index) const {
    const auto& obj = values_[index];
    return MapReduce::GetValueScriptObject(obj);
}

const rs::scriptobject::ScriptArrayPtr ScriptArrayJsapiSource::getArray(int index) const {
    const auto& arr = values_[index];
    return MapReduce::GetValueScriptArray(arr);
}