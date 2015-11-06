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

#include "map_reduce_script_object_state.h"
#include "map_reduce.h"

ScriptArrayJsapiKeyValueSource ScriptArrayJsapiKeyValueSource::Create(const rs::jsapi::Value& key, const rs::jsapi::Value& value) {
    ScriptArrayJsapiKeyValueSource source{key, value};
    
    auto cx = key.getContext();

    for (decltype(source.values_.size()) i = 0, length = source.values_.size(); i < length; ++i) {
        switch (JS_TypeOfValue(cx, source.values_[i])) {
            case JSTYPE_OBJECT: 
                if (source.values_[i].isArray() || rs::jsapi::DynamicArray::IsDynamicArray(source.values_[i])) {
                    source.types_[i] = ScriptObjectType::Array;
                } else if (!source.values_[i].isNull()) {
                    source.types_[i] = ScriptObjectType::Object;
                } else {
                    source.types_[i] = ScriptObjectType::Null;
                }
                break;

            case JSTYPE_STRING:
                source.types_[i] = ScriptObjectType::String;
                source.stringValues_[i] = std::move(source.values_[i].ToString());
                break;

            case JSTYPE_NUMBER: source.types_[i] = ScriptObjectType::Double; break;
            case JSTYPE_BOOLEAN: source.types_[i] = ScriptObjectType::Boolean; break;
            default: source.types_[i] = ScriptObjectType::Null; break;
        }
    }
    
    return source;
}

ScriptArrayJsapiKeyValueSource::ScriptArrayJsapiKeyValueSource(const rs::jsapi::Value& key, const rs::jsapi::Value& value) :
        values_{ key, value }, types_{ ScriptObjectType::Unknown, ScriptObjectType::Unknown } {
    
}

ScriptArrayJsapiKeyValueSource::ScriptArrayJsapiKeyValueSource(ScriptArrayJsapiKeyValueSource&& rhs) :
        values_(std::move(rhs.values_)),
        types_(std::move(rhs.types_)), stringValues_(std::move(rhs.stringValues_)) {
    
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
    const auto& obj = values_[index];
    return MapReduce::GetValueScriptObject(obj);
}

const rs::scriptobject::ScriptArrayPtr ScriptArrayJsapiKeyValueSource::getArray(int index) const {
    const auto& arr = values_[index];
    return MapReduce::GetValueScriptArray(arr);
}