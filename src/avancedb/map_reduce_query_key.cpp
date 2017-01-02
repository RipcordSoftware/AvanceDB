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

#include "map_reduce_query_key.h"

#include <cstring>

#include "script_array_json_source.h"

MapReduceQueryKey::MapReduceQueryKey(script_array_ptr key, const char* id) :
        key_(key), id_(id ? id : "") {
    
}

map_reduce_query_key_ptr MapReduceQueryKey::Create(const char* json, const char* id) {
    auto length = std::strlen(json ? json : "");
    
    std::vector<char> arr;
    arr.reserve(length + 2 + 1);
    arr.push_back('[');
    arr.insert(arr.end(), json, json + length);
    arr.push_back(']');
    arr.push_back('\0');
    
    rs::scriptobject::ScriptArrayJsonSource source{arr.data()};
    auto key = rs::scriptobject::ScriptArrayFactory::CreateArray(source);
    
    return boost::make_shared<MapReduceQueryKey>(key, id);
}

script_array_ptr MapReduceQueryKey::GetKeyArray() const {
    return key_;
}

const char* MapReduceQueryKey::getId() const {
    return id_.c_str();
}

unsigned MapReduceQueryKey::getCount() const {
    return 1;
}

rs::scriptobject::ScriptObjectType MapReduceQueryKey::getType(int) const {
    return key_->getType(KeyIndex);
}

const char* MapReduceQueryKey::getString(int) const {
    return key_->getString(KeyIndex);
}

std::int32_t MapReduceQueryKey::getInt32(int) const {
    return key_->getInt32(KeyIndex);
}

double MapReduceQueryKey::getDouble(int) const {
    return key_->getDouble(KeyIndex);
}

bool MapReduceQueryKey::getBoolean(int) const {
    return key_->getBoolean(KeyIndex);
}

const script_object_ptr MapReduceQueryKey::getObject(int) const {
    return key_->getObject(KeyIndex);
}

const script_array_ptr MapReduceQueryKey::getArray(int) const {
    return key_->getArray(KeyIndex);
}