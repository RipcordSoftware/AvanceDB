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
#include "documents_collection.h"
#include "get_view_options.h"
#include "map_reduce_results_iterator.h"

class MapReduceResults final {
public:
    
    MapReduceResults(map_reduce_result_array_ptr results);
    MapReduceResults(const GetViewOptions& options, map_reduce_result_array_ptr results);    
    
    DocumentsCollection::size_type Limit() const;
    DocumentsCollection::size_type Skip() const;
    DocumentsCollection::size_type Offset() const;
    DocumentsCollection::size_type TotalRows() const;
    
    MapReduceResultsIterator Iterator() const;
    
private:
    
    const DocumentsCollection::size_type FindMissedFlag = ~(std::numeric_limits<DocumentsCollection::size_type>::max() / 2);
    
    static DocumentsCollection::size_type FindResult(const map_reduce_result_array& results, const map_reduce_query_key_ptr key);
    
    static DocumentsCollection::size_type Subtract(DocumentsCollection::size_type, DocumentsCollection::size_type);
    
    const map_reduce_result_array_ptr results_;
    bool inclusiveEnd_;
    bool descending_;
    DocumentsCollection::size_type startIndex_;
    DocumentsCollection::size_type endIndex_;
    DocumentsCollection::size_type limit_;
    DocumentsCollection::size_type skip_;
};

#endif	/* MAP_REDUCE_RESULTS_H */

