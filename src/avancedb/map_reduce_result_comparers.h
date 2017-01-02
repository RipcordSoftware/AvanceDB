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

#ifndef RS_AVANCEDB_MAP_REDUCE_RESULT_COMPARERS_H
#define RS_AVANCEDB_MAP_REDUCE_RESULT_COMPARERS_H

#include "types.h"

#include <cstring>
#include <algorithm>

#include "map_reduce_result.h"

class MapReduceResultComparers final {
public:
    MapReduceResultComparers() = delete;
    
    template <typename T, typename std::enable_if<std::is_same<T, map_reduce_result_ptr>::value>::type* = nullptr>
    static int Compare(const T& a, const T& b) {
        MapReduceResultKeyAdapter tempA{a}, tempB{b};
        
        auto diff = CompareImpl(&tempA, &tempB);
        if (diff == 0) {
            diff = a->CompareId(*b);
        }
        
        return diff;
    }
    
    template <typename T, typename std::enable_if<std::is_same<T, map_reduce_result_ptr>::value>::type* = nullptr>
    static bool Less(const T& a, const T& b) {
        return Compare(a, b) < 0;
    }

    template <typename T, typename std::enable_if<std::is_same<T, script_object_ptr>::value || std::is_same<T, script_array_ptr>::value>::type* = nullptr>
    static int Compare(const T& a, const T& b) {
        return CompareImpl(a, b);
    }
    
    template <typename T, typename std::enable_if<std::is_same<T, script_object_ptr>::value || std::is_same<T, script_array_ptr>::value>::type* = nullptr>
    static bool Less(const T& a, const T& b) {
        return Compare(a, b) < 0;
    }
    
    template <typename T, typename std::enable_if<std::is_same<T, script_object_ptr>::value || std::is_same<T, script_array_ptr>::value>::type* = nullptr>
    static int CompareField(unsigned index, const T& a, const T& b) {
        return CompareValueImpl(index, a, b);
    }
    
    static inline int GetScriptObjectTypePrecedence(const rs::scriptobject::ScriptObjectType& type) {
        using ScriptObjectType = rs::scriptobject::ScriptObjectType;

        switch (type) {
            case ScriptObjectType::Null: return 0;
            case ScriptObjectType::Boolean: return 1;
            case ScriptObjectType::Int32: return 2;
            case ScriptObjectType::Double: return 2;
            case ScriptObjectType::String: return 3;
            case ScriptObjectType::Array: return 4;
            case ScriptObjectType::Object: return 5;
            default: return 0;
        }
    }
    
private:
    class MapReduceResultKeyAdapter final {
    public:
        MapReduceResultKeyAdapter(const map_reduce_result_ptr&);
        
        rs::scriptobject::ScriptObjectType getType(int) const;
        inline unsigned getCount() const { return 1; }

        const char* getString(int) const;
        std::int32_t getInt32(int) const;
        double getDouble(int) const;
        bool getBoolean(int) const;
        const script_object_ptr getObject(int) const;
        const script_array_ptr getArray(int) const;
        
    private:
        const map_reduce_result_ptr& result_;
    };
    
    template <typename T, typename std::enable_if<std::is_same<T, script_array_ptr>::value || std::is_same<T, MapReduceResultKeyAdapter*>::value>::type* = nullptr>
    static int CompareImpl(const T& a, const T& b) {        
        using ScriptObjectType = rs::scriptobject::ScriptObjectType;
        
        int compare = 0;
        auto countA = a->getCount();
        auto countB = b->getCount();
        auto count = std::min(countA, countB);

        for (decltype(count) i = 0; compare == 0 && i < count; ++i) {
            compare = CompareValueImpl(i, a, b);
        }

        if (compare == 0) {
            compare = countA - countB;
        }

        return compare;
    }
    
    template <typename T, typename std::enable_if<std::is_same<T, script_object_ptr>::value>::type* = nullptr>
    static int CompareImpl(const T& a, const T& b) {        
        using ScriptObjectType = rs::scriptobject::ScriptObjectType;
        
        int compare = 0;
        auto countA = a->getCount();
        auto countB = b->getCount();
        auto count = std::min(countA, countB);

        for (decltype(count) i = 0; compare == 0 && i < count; ++i) {
            auto nameA = a->getName(i);
            auto nameB = b->getName(i);
            compare = std::strcmp(nameA, nameB);
            
            if (compare == 0) {
                compare = CompareValueImpl(i, a, b);
            }
        }

        if (compare == 0) {
            compare = countA - countB;
        }

        return compare;
    }
    
    template <typename T>
    static int CompareValueImpl(unsigned index, const T& a, const T& b) {
        using ScriptObjectType = rs::scriptobject::ScriptObjectType;

        int compare = 0;
        auto typeA = a->getType(index);
        auto typeB = b->getType(index);

        if (typeA != typeB) {
            compare = GetScriptObjectTypePrecedence(typeA) - GetScriptObjectTypePrecedence(typeB);
        } else {
            switch (typeA) {
                case ScriptObjectType::Null: compare = 0; break;
                case ScriptObjectType::Boolean: compare = CompareBoolean(a->getBoolean(index), b->getBoolean(index)); break;
                case ScriptObjectType::Int32: compare = a->getInt32(index) - b->getInt32(index); break;
                case ScriptObjectType::Double: compare = CompareDouble(a->getDouble(index), b->getDouble(index)); break;
                case ScriptObjectType::String: compare = std::strcmp(a->getString(index), b->getString(index)); break;
                case ScriptObjectType::Object: compare = CompareImpl(a->getObject(index), b->getObject(index)); break;
                case ScriptObjectType::Array: compare = CompareImpl(a->getArray(index), b->getArray(index)); break;
            }
        }

        return compare;
    }
    
    static inline  int CompareDouble(double a, double b) {
        return a < b ? -1 : (a > b ? 1 : 0);
    }
    
    static inline int CompareBoolean(bool a, bool b) {
        return a == b ? 0 : (a == false ? -1 : 1);
    }
};

#endif /* RS_AVANCEDB_MAP_REDUCE_RESULT_COMPARERS_H */

