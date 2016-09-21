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

#ifndef RS_AVANCEDB_MAP_REDUCE_RESULTS_ITERATOR_H
#define RS_AVANCEDB_MAP_REDUCE_RESULTS_ITERATOR_H

#include "map_reduce_results.h"

class MapReduceResultsIterator final {
public:
    using const_reference = MapReduceResults::const_reference;
    
    MapReduceResultsIterator(const MapReduceResults& results, bool descending);
    
    const_reference Next();

private:    
    
    const MapReduceResults& results_;
    const MapReduceResults::const_iterator begin_;
    const MapReduceResults::const_iterator end_;
    const bool empty_;
    const MapReduceResults::const_iterator ibegin_;
    const MapReduceResults::const_iterator iend_;
    MapReduceResults::const_iterator iter_;
    const int direction_;
    const MapReduceResults::value_type null_{nullptr};
};

#endif /* RS_AVANCEDB_MAP_REDUCE_RESULTS_ITERATOR_H */

