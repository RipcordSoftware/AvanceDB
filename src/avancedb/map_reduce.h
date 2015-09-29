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

#ifndef MAP_REDUCE_H
#define MAP_REDUCE_H

#include "types.h"

#include "libjsapi.h"

class MapReduce final {
public:
    
    MapReduce() = delete;
    
    static map_reduce_result_array_ptr Execute(const char* map, const char* reduce, document_array_ptr docs);
    
private:
    
    // TODO: merge with other type mapping functions where possible
    static void GetFieldValue(script_object_ptr scriptObj, const char* name, rs::jsapi::Value& value);
    static void GetFieldValue(script_array_ptr scriptObj, int index, rs::jsapi::Value& value);

};

#endif	/* MAP_REDUCE_H */

