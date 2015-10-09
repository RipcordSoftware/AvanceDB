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

#ifndef MAP_REDUCE_RESULT_COMPARERS_H
#define MAP_REDUCE_RESULT_COMPARERS_H

#include "types.h"

#include <cstring>
#include <algorithm>

#include "map_reduce_result.h"

class MapReduceResultComparers final {
public:    
    class MapReduceResultKeyAdapter final {
    public:
        MapReduceResultKeyAdapter(const map_reduce_result_ptr&);
        
        rs::scriptobject::ScriptObjectType getType(int) const;
        unsigned getCount() const;

        const char* getString(int) const;
        std::int32_t getInt32(int) const;
        double getDouble(int) const;
        bool getBoolean(int) const;
        const script_object_ptr getObject(int) const;
        const script_array_ptr getArray(int) const;
        
    private:
        const map_reduce_result_ptr& result_;
    };
    
    MapReduceResultComparers() = delete;
    
    template <typename T, typename std::enable_if<std::is_same<T, script_object_ptr>::value || std::is_same<T, script_array_ptr>::value || std::is_same<T, MapReduceResultKeyAdapter*>::value>::type* = nullptr>
    static bool Compare(const T& a, const T& b) {
        using ScriptObjectType = rs::scriptobject::ScriptObjectType;
        auto compare = false;

        auto countA = a->getCount();
        auto countB = a->getCount();
        auto count = std::min(countA, countB);

        for (decltype(count) i = 0; i < count; ++i) {
            auto typeA = a->getType(i);
            auto typeB = b->getType(i);

            if (typeA != typeB) {
                return GetScriptObjectTypePrecedence(typeA) < GetScriptObjectTypePrecedence(typeB);
            } else {
                switch (typeA) {
                    case ScriptObjectType::Null: compare = false; break;
                    case ScriptObjectType::Boolean: compare = a->getBoolean(i) < b->getBoolean(i); break;
                    case ScriptObjectType::Int32: compare = a->getInt32(i) < b->getInt32(i); break;
                    case ScriptObjectType::Double: compare = a->getDouble(i) < b->getDouble(i); break;
                    case ScriptObjectType::String: compare = std::strcmp(a->getString(i), b->getString(i)) < 0; break;
                    case ScriptObjectType::Object: compare = Compare(a->getObject(i), b->getObject(i)) < 0; break;
                    case ScriptObjectType::Array: compare = Compare(a->getArray(i), b->getArray(i)) < 0; break;
                }
            }
        }

        if (!compare) {
            compare = countA - countB;
        }

        return compare;
    }
    
    static int GetScriptObjectTypePrecedence(const rs::scriptobject::ScriptObjectType& type);
};

#endif	/* MAP_REDUCE_RESULT_COMPARERS_H */

