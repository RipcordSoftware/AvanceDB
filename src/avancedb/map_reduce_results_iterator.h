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

#ifndef MAP_REDUCE_RESULTS_ITERATOR_H
#define MAP_REDUCE_RESULTS_ITERATOR_H

#include <type_traits>

#include "types.h"
#include "documents_collection.h"

class MapReduceResultsIterator final {
public:
    MapReduceResultsIterator(map_reduce_result_array_ptr results, 
            DocumentsCollection::size_type startIndex, 
            DocumentsCollection::size_type endIndex, 
            DocumentsCollection::size_type limit, 
            bool inclusiveEnd, 
            bool descending);

    map_reduce_result_array_ptr::element_type::const_reference operator++() const;
    map_reduce_result_array_ptr::element_type::const_reference operator++(int) const;
    map_reduce_result_array_ptr::element_type::const_reference operator*() const;

private:
    bool CheckIndex(DocumentsCollection::size_type) const;
    static DocumentsCollection::size_type CalculateEndIndex(DocumentsCollection::size_type startIndex, DocumentsCollection::size_type endIndex, DocumentsCollection::size_type size, bool descending);
    
    const map_reduce_result_array_ptr results_;
    const bool inclusiveEnd_;
    const bool descending_;
    const DocumentsCollection::size_type startIndex_;
    const DocumentsCollection::size_type endIndex_;
    mutable DocumentsCollection::size_type index_;
    mutable std::make_signed<DocumentsCollection::size_type>::type limit_;
    mutable map_reduce_result_array_ptr::element_type::value_type ptr_;
};

#endif	/* MAP_REDUCE_RESULTS_ITERATOR_H */

