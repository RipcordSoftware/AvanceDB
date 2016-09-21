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

#ifndef MAP_REDUCE_QUERY_KEY_H
#define MAP_REDUCE_QUERY_KEY_H

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "types.h"

class MapReduceQueryKey final : public boost::enable_shared_from_this<MapReduceQueryKey>, private boost::noncopyable {
public:
    
    static constexpr unsigned KeyIndex{0};
    
    static map_reduce_query_key_ptr Create(const char* json, const char* id);
    
    script_array_ptr GetKeyArray() const;
    
    const char* getId() const;
    
    rs::scriptobject::ScriptObjectType getType(int) const;
    unsigned getCount() const;
    const char* getString(int) const;
    std::int32_t getInt32(int) const;
    double getDouble(int) const;
    bool getBoolean(int) const;
    const script_object_ptr getObject(int) const;
    const script_array_ptr getArray(int) const;
    
private:
    
    friend map_reduce_query_key_ptr boost::make_shared<map_reduce_query_key_ptr::element_type>(script_array_ptr&, const char*&);
    
    MapReduceQueryKey(script_array_ptr, const char*);

    const script_array_ptr key_;
    const std::string id_;
};

#endif	/* MAP_REDUCE_QUERY_KEY_H */

