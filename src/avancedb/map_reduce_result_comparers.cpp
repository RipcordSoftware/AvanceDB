/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2016 Ripcord Software
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

#include "map_reduce_result_comparers.h"

MapReduceResultComparers::MapReduceResultKeyAdapter::MapReduceResultKeyAdapter(const map_reduce_result_ptr& result) :
    result_(result) {    
}

rs::scriptobject::ScriptObjectType MapReduceResultComparers::MapReduceResultKeyAdapter::getType(int) const {
    return result_->getKeyType();
}

const char* MapReduceResultComparers::MapReduceResultKeyAdapter::getString(int) const {
    return result_->getKeyString();
}

std::int32_t MapReduceResultComparers::MapReduceResultKeyAdapter::getInt32(int) const {
    return result_->getKeyInt32();
}

double MapReduceResultComparers::MapReduceResultKeyAdapter::getDouble(int) const {
    return result_->getKeyDouble();
}

bool MapReduceResultComparers::MapReduceResultKeyAdapter::getBoolean(int) const {
    return result_->getKeyBoolean();
}

const script_object_ptr MapReduceResultComparers::MapReduceResultKeyAdapter::getObject(int) const {
    return result_->getKeyObject();
}

const script_array_ptr MapReduceResultComparers::MapReduceResultKeyAdapter::getArray(int) const {
    return result_->getKeyArray();
}