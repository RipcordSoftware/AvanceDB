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

#include "map_reduce_result.h"

#include <cstring>

#include "document.h"
#include "map_reduce_result_comparers.h"

const unsigned MapReduceResult::KeyIndex = 0;
const unsigned MapReduceResult::ValueIndex = 1;

MapReduceResult::MapReduceResult(script_array_ptr result, document_ptr doc) :
        result_(result), doc_(doc) {
}

map_reduce_result_ptr MapReduceResult::Create(script_array_ptr result, document_ptr doc) {    
    return boost::make_shared<MapReduceResult>(result, doc);
}

const char* MapReduceResult::MapReduceResult::getId() const {
    return doc_->getId();
}

rs::scriptobject::ScriptObjectType MapReduceResult::getKeyType() const {
    return result_->getType(KeyIndex);
}

rs::scriptobject::ScriptObjectType MapReduceResult::getValueType() const {
    return result_->getType(ValueIndex);
}

const document_ptr MapReduceResult::getDoc() const {
    return doc_;
}

const script_array_ptr MapReduceResult::getResultArray() const {
    return result_;
}

const char* MapReduceResult::getKeyString() const {
    return result_->getString(KeyIndex);
}

std::int32_t MapReduceResult::getKeyInt32() const {
    return result_->getInt32(KeyIndex);
}

double MapReduceResult::getKeyDouble() const {
    return result_->getDouble(KeyIndex);
}

bool MapReduceResult::getKeyBoolean() const {
    return result_->getBoolean(KeyIndex);
}

const script_object_ptr MapReduceResult::getKeyObject() const {
    return result_->getObject(KeyIndex);
}

const script_array_ptr MapReduceResult::getKeyArray() const {
    return result_->getArray(KeyIndex);
}

const char* MapReduceResult::getValueString() const {
    return result_->getString(ValueIndex);
}

std::int32_t MapReduceResult::getValueInt32() const {
    return result_->getInt32(ValueIndex);
}

double MapReduceResult::getValueDouble() const {
    return result_->getDouble(ValueIndex);
}

bool MapReduceResult::getValueBoolean() const {
    return result_->getBoolean(ValueIndex);
}

const script_object_ptr MapReduceResult::getValueObject() const {
    return result_->getObject(ValueIndex);
}

const script_array_ptr MapReduceResult::getValueArray() const {
    return result_->getArray(ValueIndex);
}

bool MapReduceResult::Compare(const map_reduce_result_ptr& a, const map_reduce_result_ptr& b) {
    MapReduceResultComparers::MapReduceResultKeyAdapter tempA{a}, tempB{b};
    
    auto compare = MapReduceResultComparers::Compare(&tempA, &tempB);
    if (!compare) {
        compare = std::strcmp(a->getId(), b->getId()) < 0;
    }
    
    return compare;
}

bool MapReduceResult::Compare(const script_object_ptr& a, const script_object_ptr& b) {
    return MapReduceResultComparers::Compare(a, b);
}

bool MapReduceResult::Compare(const script_array_ptr& a, const script_array_ptr& b) {
    return MapReduceResultComparers::Compare(a, b);
}