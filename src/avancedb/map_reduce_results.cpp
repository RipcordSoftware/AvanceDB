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
#include "map_reduce_result_comparers.h"
#include "map_reduce_query_key.h"

#include <algorithm>
#include <cstring>

MapReduceResults::MapReduceResults(map_reduce_result_array_ptr results) :
        results_(results), startIndex_(0), endIndex_(results->size()),
        limit_(results->size()), skip_(0) {
    
}

size_t MapReduceResults::Offset() const {
    return startIndex_ + skip_;
}

size_t MapReduceResults::TotalRows() const {
    return results_->size();
}

map_reduce_result_array_ptr::element_type::const_iterator MapReduceResults::cbegin() const {
    auto startIndex = 0;
    if (startIndex_ > 0) {
        startIndex = std::min(startIndex_, TotalRows());
    }
    
    if (skip_ > 0) {
        startIndex = std::min(startIndex + skip_, TotalRows());
    }
    
    return results_->cbegin() + startIndex;
}

map_reduce_result_array_ptr::element_type::const_iterator MapReduceResults::cend() const {
    auto endIndex = endIndex_;
    
    if ((startIndex_ + skip_ + limit_) < endIndex) {
        endIndex = startIndex_ + skip_ + limit_;
    }
    
    return cbegin() + endIndex;
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

void MapReduceResults::StartKey(map_reduce_query_key_ptr key) {
    startIndex_ = 0;
    
    if (!!key) {        
        auto compare = [](map_reduce_result_ptr result, map_reduce_query_key_ptr startKey) {
            auto diff = MapReduceResultComparers::CompareField(0, result->getResultArray(), startKey->GetKeyArray());
            if (diff == 0) {
                auto startKeyId = startKey->getId();
                if (startKeyId && startKeyId[0] != '\0') {
                    diff = std::strcmp(result->getId(), startKeyId);
                }
            }
            return diff < 0;
        };

        auto iter = std::lower_bound(results_->cbegin(), results_->cend(), key, compare);
        startIndex_ = std::distance(results_->cbegin(), iter);
    }
}

void MapReduceResults::EndKey(map_reduce_query_key_ptr key, bool inclusiveEnd) {
    endIndex_ = TotalRows();
    
    if (!!key) {        
        auto compare = [](map_reduce_result_ptr result, map_reduce_query_key_ptr endKey) {
            auto diff = MapReduceResultComparers::CompareField(0, result->getResultArray(), endKey->GetKeyArray());
            if (diff == 0) {
                auto startKeyId = endKey->getId();
                if (startKeyId && startKeyId[0] != '\0') {
                    diff = std::strcmp(result->getId(), startKeyId);
                }
            }
            return diff < 0;
        };

        auto iter = std::lower_bound(results_->cbegin(), results_->cend(), key, compare);
        endIndex_ = std::distance(results_->cbegin(), iter);
        
        if (inclusiveEnd && endIndex_ < TotalRows()) {
            ++endIndex_;
        }
    }
}