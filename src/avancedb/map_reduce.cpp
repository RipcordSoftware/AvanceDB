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

#include "map_reduce.h"

#include <boost/make_shared.hpp>

#include "script_array_jsapi_key_value_source.h"
#include "map_reduce_result.h"
#include "documents.h"

#include "libjsapi.h"

map_reduce_result_array_ptr MapReduce::Execute(const char* map, const char* reduce, document_array_ptr docs) {
    map_reduce_result_array_ptr results = boost::make_shared<map_reduce_result_array_ptr::element_type>();
    
    // create the function script
    std::string mapScript = "(function() { return ";
    mapScript += map;
    mapScript += "; })();";

    // create a runtime on this thread
    rs::jsapi::Runtime rt;
    
    document_ptr doc = nullptr;

    // define a function in global scope implemented by a C++ lambda
    rs::jsapi::Global::DefineFunction(rt, "emit", 
        [&](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value&) {
            auto source = ScriptArrayJsapiKeyValueSource::Create(args[0], args[1]);
            
            auto resultArr = rs::scriptobject::ScriptArrayFactory::CreateArray(source);
            auto result = MapReduceResult::Create(resultArr, doc);
            results->push_back(result);
    });

    // TODO: elegantly handle JS syntax errors
    // execute the script in the context of the runtime, getting the resulting function
    rs::jsapi::Value func(rt);
    rt.Evaluate(mapScript.c_str(), func);

    rs::scriptobject::ScriptObjectPtr scriptObj = nullptr;
    rs::jsapi::Value object(rt);
    rs::jsapi::DynamicObject::Create(rt, 
        [&](const char* name, rs::jsapi::Value& value) {
            return MapReduce::GetFieldValue(scriptObj, name, value);
        }, 
        nullptr, 
        [&](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>&) {
            // TODO: only enumerate when really needed
            for (decltype(scriptObj->getCount()) i = 0, count = scriptObj->getCount(); i < count; ++i) {
                props.emplace_back(scriptObj->getName(i));
            }
            return true;
        }, 
        nullptr, object);

    rs::jsapi::FunctionArguments args(rt);
    args.Append(object);

    for (Documents::collection::size_type i = 0, size = docs->size(); i < size; ++i) {
        doc = (*docs)[i];
        scriptObj = doc->getObject();

        // TODO: handle exception cases here
        func.CallFunction(args);
    }
    
    return results;
}

// TODO: merge with other type mapping functions where possible
void MapReduce::GetFieldValue(script_object_ptr scriptObj, const char* name, rs::jsapi::Value& value) {
    switch (scriptObj->getType(name)) {
        case rs::scriptobject::ScriptObjectType::Boolean:
            value = scriptObj->getBoolean(name);
            return;
        case rs::scriptobject::ScriptObjectType::Int32:
            value = scriptObj->getInt32(name);
            return;
        case rs::scriptobject::ScriptObjectType::String:
            value = scriptObj->getString(name);
            return;
        case rs::scriptobject::ScriptObjectType::Double:
            value = scriptObj->getDouble(name);
            return;
        case rs::scriptobject::ScriptObjectType::Object: {
            auto childObj = scriptObj->getObject(name);
            
            auto cx = value.getContext();
            rs::jsapi::DynamicObject::Create(cx, 
                [childObj](const char* name, rs::jsapi::Value& value) {                    
                    return MapReduce::GetFieldValue(childObj, name, value);
                }, 
                nullptr, 
                [childObj](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>&) {
                    for (decltype(childObj->getCount()) i = 0, count = childObj->getCount(); i < count; ++i) {
                        props.emplace_back(childObj->getName(i));
                    }
                    return true;
                }, nullptr, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Array: {
            auto childArr = scriptObj->getArray(name);
            
            auto cx = value.getContext();            
            rs::jsapi::DynamicArray::Create(cx, 
                [childArr](int index, rs::jsapi::Value& value) {                    
                    return MapReduce::GetFieldValue(childArr, index, value);
                }, 
                nullptr, [childArr]() { return childArr->getCount(); }, nullptr, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Null:
            value = JS::NullHandleValue;
            return;
        default:
            value = JS::UndefinedHandleValue;
            return;
    }
}

// TODO: merge with other type mapping functions where possible
void MapReduce::GetFieldValue(script_array_ptr scriptArr, int index, rs::jsapi::Value& value) {
    switch (scriptArr->getType(index)) {
        case rs::scriptobject::ScriptObjectType::Boolean:
            value = scriptArr->getBoolean(index);
            return;
        case rs::scriptobject::ScriptObjectType::Int32:
            value = scriptArr->getInt32(index);
            return;
        case rs::scriptobject::ScriptObjectType::String:
            value = scriptArr->getString(index);
            return;
        case rs::scriptobject::ScriptObjectType::Double:
            value = scriptArr->getDouble(index);
            return;
        case rs::scriptobject::ScriptObjectType::Object: {
            auto childObj = scriptArr->getObject(index);
            
            auto cx = value.getContext();
            rs::jsapi::DynamicObject::Create(cx, 
                [childObj](const char* name, rs::jsapi::Value& value) {                    
                    return MapReduce::GetFieldValue(childObj, name, value);
                }, 
                nullptr, 
                [childObj](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>&) {
                    for (decltype(childObj->getCount()) i = 0, count = childObj->getCount(); i < count; ++i) {
                        props.emplace_back(childObj->getName(i));
                    }
                    return true;
                }, nullptr, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Array: {
            auto childArr = scriptArr->getArray(index);

            auto cx = value.getContext();
            rs::jsapi::DynamicArray::Create(cx, 
                [childArr](int index, rs::jsapi::Value& value) {                    
                    return MapReduce::GetFieldValue(childArr, index, value);
                }, 
                nullptr, [childArr]() { return childArr->getCount(); }, nullptr, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Null:
            value = JS::NullHandleValue;
            return;
        default:
            value = JS::UndefinedHandleValue;
    }
}