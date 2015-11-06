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

#ifndef MAP_REDUCE_RESULT_H
#define MAP_REDUCE_RESULT_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <cstring>

#include "types.h"

class MapReduceResult final : public boost::enable_shared_from_this<MapReduceResult>, private boost::noncopyable {
public:
    
    static constexpr unsigned KeyIndex{0};
    static constexpr unsigned ValueIndex{1};
    
    static map_reduce_result_ptr Create(script_array_ptr result, document_ptr doc);
    
    const char* getId() const;
    const document_ptr getDoc() const;
    const script_array_ptr getResultArray() const;
    
    rs::scriptobject::ScriptObjectType getKeyType() const;
    rs::scriptobject::ScriptObjectType getValueType() const;        
    
    const char* getKeyString() const;
    std::int32_t getKeyInt32() const;
    double getKeyDouble() const;
    bool getKeyBoolean() const;
    const script_object_ptr getKeyObject() const;
    const script_array_ptr getKeyArray() const;
    
    const char* getValueString() const;
    std::int32_t getValueInt32() const;
    double getValueDouble() const;
    bool getValueBoolean() const;
    const script_object_ptr getValueObject() const;
    const script_array_ptr getValueArray() const;
    
    inline int CompareId(const MapReduceResult& other) {
        return std::strcmp(id_, other.id_);
    }
    
    static bool Less(const map_reduce_result_ptr& a, const map_reduce_result_ptr& b);
    static bool Less(const script_object_ptr& a, const script_object_ptr& b);
    static bool Less(const script_array_ptr& a, const script_array_ptr& b);
    
private:
    
    friend map_reduce_result_ptr boost::make_shared<map_reduce_result_ptr::element_type>(script_array_ptr&&, document_ptr&&);
    
    MapReduceResult(script_array_ptr&&, document_ptr&&);
    
    const char* id_;
    document_ptr doc_;
    script_array_ptr result_;

};

#endif	/* MAP_REDUCE_RESULT_H */

