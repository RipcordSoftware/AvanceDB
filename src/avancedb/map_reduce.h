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

#include <string>

#include "types.h"
#include "map_reduce_results.h"
#include "map_reduce_thread_pool.h"

#include "libjsapi.h"

class GetViewOptions;

class MapReduce final {
public:
    
    class MapReduceTask final {
    public:
        static inline MapReduceTask Create(script_object_ptr optionsObj) {
            auto lang = optionsObj->getString("language", false);
            auto map = optionsObj->getString("map", false);
            auto reduce = optionsObj->getString("reduce", false);
            
            MapReduceTask options{lang, map, reduce};
            return options;            
        }
        
        const char* Map() const { return map_.c_str(); }
        const char* Reduce() const { return reduce_.c_str(); }
        const char* Language() const { return language_.c_str(); }
        
    private:
        MapReduceTask(const char* language, const char* map, const char* reduce) :
                language_(language ? language : "javascript"), 
                map_(map ? map : ""), 
                reduce_(reduce ? reduce : "") {}
        
        const std::string map_;
        const std::string reduce_;
        const std::string language_;
    };
    
    MapReduce();
    
    map_reduce_results_ptr Execute(const GetViewOptions& options, const MapReduceTask& task, document_collections_ptr_array colls);
    
    static script_object_ptr GetValueScriptObject(const rs::jsapi::Value& value);
    static script_array_ptr GetValueScriptArray(const rs::jsapi::Value& value);
    
private:
    
    map_reduce_result_array_ptr Execute(rs::jsapi::Runtime& rt, const MapReduceTask& task, const document_array& docs);
    
    static void GetFieldValue(script_object_ptr scriptObj, const char* name, rs::jsapi::Value& value);
    static void GetFieldValue(script_array_ptr scriptObj, int index, rs::jsapi::Value& value);
    
    static void CreateValueObject(script_object_ptr obj, rs::jsapi::Value& value);
    static void CreateValueArray(script_array_ptr arr, rs::jsapi::Value& value);
    
    static void SortResultArray(map_reduce_result_array_ptr results);
    
    MapReduceThreadPool::map_reduce_thread_pool_ptr mapReduceThreadPool_;
};

#endif	/* MAP_REDUCE_H */

