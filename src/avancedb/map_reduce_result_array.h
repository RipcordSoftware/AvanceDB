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

#ifndef MAP_REDUCE_RESULT_ARRAY_H
#define MAP_REDUCE_RESULT_ARRAY_H

#include <vector>

#include "types.h"
#include "map_reduce_result.h"

class MapReduceResultArray final {        
public:
    using collection = std::vector<map_reduce_result_ptr>;
    using size_type = collection::size_type;
    using value_type = collection::value_type;
    using const_iterator = collection::const_iterator;
    using const_reverse_iterator = collection::const_reverse_iterator;
    using iterator = collection::iterator;
    using const_reference = collection::const_reference;
    
    MapReduceResultArray(collection::size_type capacity = 1024);
    MapReduceResultArray(const MapReduceResultArray&) = delete;
    MapReduceResultArray(MapReduceResultArray&& rhs);
    ~MapReduceResultArray();
    
    size_type capacity() const;
    void reserve(size_type n);
    void resize(size_type n);
    size_type size() const;
    void clear();
    
    const_iterator cbegin() const;
    const_iterator cend() const;
    
    iterator begin();
    iterator end();
    
    void insert(iterator position, const_iterator first, const_iterator last, const map_reduce_result_array_ptr& sourcePtr);
    
    void push_back(map_reduce_result_ptr);
    
    collection& operator=(const collection&) = delete;
    const_reference operator[](int n) const;
    
private:
    collection data_;
    
    std::vector<map_reduce_result_array_ptr> sources_;
};

#endif	/* MAP_REDUCE_RESULT_ARRAY_H */

