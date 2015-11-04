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

#include "map_reduce_results_iterator.h"

MapReduceResultsIterator::MapReduceResultsIterator(const MapReduceResults& results, bool descending) :
        results_(results),
        begin_(results.cbegin()),
        end_(results.cend()),
        empty_(begin_ == end_),
        ibegin_(!descending || empty_ ? begin_ : end_ - 1),
        iend_(!descending || empty_ ? end_ : begin_ - 1),
        iter_(ibegin_),
        direction_(!descending ? 1 : -1) {
    
}

MapReduceResultsIterator::const_reference MapReduceResultsIterator::Next() {
    if (!empty_ && iter_ != iend_) {
        const auto& value = *iter_;
        iter_ += direction_;
        return value;
    } else {
        return null_;
    }
}