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

#ifndef MAP_REDUCE_RESULTS_H
#define MAP_REDUCE_RESULTS_H

#include "types.h"

#include <limits>

class MapReduceResults {
public:
    
    MapReduceResults(map_reduce_result_array_ptr);
    
    void StartKey(map_reduce_query_key_ptr key);
    void EndKey(map_reduce_query_key_ptr key, bool inclusiveEnd);
    
    void Limit(size_t);
    size_t Limit() const;
    
    void Skip(size_t);
    size_t Skip() const;

    size_t Offset() const;
    size_t TotalRows() const;
    
    map_reduce_result_array_ptr::element_type::const_iterator cbegin() const;
    map_reduce_result_array_ptr::element_type::const_iterator cend() const;
    
private:
    
    const map_reduce_result_array_ptr results_;
    size_t startIndex_;
    size_t endIndex_;
    size_t limit_;
    size_t skip_;
};

#endif	/* MAP_REDUCE_RESULTS_H */

