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

#include <limits>

#include "types.h"
#include "document_collection.h"
#include "get_view_options.h"

class MapReduceResultsIterator;

class MapReduceResults final {
public:
    using value_type = map_reduce_result_array_ptr::element_type::value_type;
    using const_reference = map_reduce_result_array_ptr::element_type::const_reference;
    using const_iterator = map_reduce_result_array_ptr::element_type::const_iterator;
    using const_reverse_iterator = map_reduce_result_array_ptr::element_type::const_reverse_iterator;
    using size_type = DocumentCollection::size_type;
    
    MapReduceResults(map_reduce_result_array_ptr results, size_type offset, size_type totalRows, size_type skip, size_type limit, size_type descending);
    
    size_type Offset() const;
    size_type FilteredRows() const;
    size_type TotalRows() const;
    
    MapReduceResultsIterator Iterator() const;
    
    const_iterator cbegin() const;
    const_iterator cend() const;    
    
private:
    
    static size_type Subtract(size_type, size_type);    
    
    const map_reduce_result_array_ptr results_;
    const bool descending_;
    const size_type limit_;
    const size_type skip_;
    const size_type offset_;
    const size_type totalRows_;
};

#endif	/* MAP_REDUCE_RESULTS_H */

