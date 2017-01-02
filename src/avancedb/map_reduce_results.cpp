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

#include "map_reduce_results.h"
#include "map_reduce_result_comparers.h"
#include "map_reduce_query_key.h"
#include "map_reduce_results_iterator.h"

#include <algorithm>
#include <cstring>

MapReduceResults::MapReduceResults(map_reduce_result_array_ptr results, size_type offset, size_type totalRows, size_type skip, size_type limit, size_type descending) :
        results_(results), skip_(std::min(skip, results->size())), limit_(std::min(limit, results->size())),
        descending_(descending), offset_(offset), totalRows_(totalRows) {
    
}

MapReduceResults::size_type MapReduceResults::Offset() const {
    return std::min(offset_ + skip_, totalRows_);
}


MapReduceResults::size_type MapReduceResults::FilteredRows() const {
    return std::distance(cbegin(), cend());
}

MapReduceResults::size_type MapReduceResults::TotalRows() const {
    return totalRows_;
}

MapReduceResultsIterator MapReduceResults::Iterator() const {
    return MapReduceResultsIterator{*this, descending_};
}

MapReduceResults::size_type MapReduceResults::Subtract(size_type a, size_type b) {
    auto v = a - b;
    if (v > a) {
        v = 0;
    }
    return v;
}

MapReduceResults::const_iterator MapReduceResults::cbegin() const {
    if (!descending_) {
        return results_->cbegin() + skip_;
    } else {
        auto startIndex = Subtract(results_->size(), skip_);
        startIndex = Subtract(startIndex, limit_);
        return results_->cbegin() + startIndex;
    }
}

MapReduceResults::const_iterator MapReduceResults::cend() const {
    if (!descending_) {    
        auto endIndex = std::min(results_->size(), skip_ + limit_);
        return results_->cbegin() + endIndex;
    } else {
        return results_->cend() - skip_;
    }
}