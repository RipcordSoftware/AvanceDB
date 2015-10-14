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

#include "map_reduce_results.h"

MapReduceResults::MapReduceResults(map_reduce_result_array_ptr results) :
        results_(results) {
    
}

size_t MapReduceResults::size() const {
    return results_->size();
}

map_reduce_result_array_ptr::element_type::const_iterator MapReduceResults::cbegin() const {
    auto begin = results_->cbegin();
    return skip_ == 0 ? begin : (skip_ < size() ? begin + skip_ : results_->cend());
}

map_reduce_result_array_ptr::element_type::const_iterator MapReduceResults::cend() const {
    return (limit_ == 0 || (skip_ + limit_) >= size()) ? results_->cend() : (cbegin() + skip_ + limit_);
}

void MapReduceResults::Limit(size_t limit) {
    limit_ = limit;
}

size_t MapReduceResults::Limit() const {
    return limit_;
}

void MapReduceResults::Skip(size_t skip) {
    skip_ = skip;
}

size_t MapReduceResults::Skip() const {
    return skip_;
}