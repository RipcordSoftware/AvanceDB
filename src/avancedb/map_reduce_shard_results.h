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

#ifndef MAP_REDUCE_SHARD_RESULTS_H
#define MAP_REDUCE_SHARD_RESULTS_H

#include <limits>

#include "types.h"
#include "document_collection.h"

class MapReduceShardResults final {
public:
    using const_iterator = map_reduce_result_array_ptr::element_type::const_iterator;
    using size_type = DocumentCollection::size_type;
    
    MapReduceShardResults(map_reduce_result_array_ptr results, size_type limit, map_reduce_query_key_ptr startKey, map_reduce_query_key_ptr endKey, bool inclusiveEnd, bool descending);
    
    size_type Offset() const;
    size_type FilteredRows() const;
    size_type TotalRows() const;
    
    const_iterator cbegin() const;
    const_iterator cend() const;    
    
private:
    
    const size_type FindMissedFlag = ~(std::numeric_limits<size_type>::max() / 2);
    
    static size_type FindResult(const map_reduce_result_array& results, const map_reduce_query_key_ptr key);
    
    static size_type Subtract(size_type, size_type);    
    
    const map_reduce_result_array_ptr results_;
    const bool inclusiveEnd_;
    const bool descending_;
    size_type startIndex_;
    size_type endIndex_;
    const size_type limit_;
};

#endif	/* MAP_REDUCE_SHARD_RESULTS_H */

