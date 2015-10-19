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
#include "map_reduce_results_iterator.h"

#include <algorithm>
#include <cstring>

MapReduceResults::MapReduceResults(map_reduce_result_array_ptr results) :
        results_(results), descending_(false), inclusiveEnd_(true),
        startIndex_(0), endIndex_(results->size()),
        limit_(results->size()), skip_(0) {
    
}

MapReduceResults::MapReduceResults(const GetViewOptions& options, map_reduce_result_array_ptr results) :
        MapReduceResults(results) {
    descending_ = options.Descending();
    endIndex_ = !descending_ ? results->size() : Subtract(results->size(), 1);
    auto inclusiveStart = descending_ ?  options.InclusiveEnd() : true;
    inclusiveEnd_ = !descending_ ?  options.InclusiveEnd() : true;
    skip_ = std::min(options.Skip(), results->size());
    limit_ = std::min(options.Limit(), results->size());
    
    auto startKey = !descending_ ? options.StartKeyObj() : options.EndKeyObj();
    auto endKey = !descending_ ? options.EndKeyObj() : options.StartKeyObj();
    
    // TODO: scan for next non-matching item
    if (!!startKey) {
        startIndex_ = FindResult(*results_, startKey);
        if (startIndex_ & FindMissedFlag) {
            startIndex_ = ~startIndex_;
        } else if (!inclusiveStart) {
            ++startIndex_;
        }
    }
    
    // TODO: scan for next non-matching item
    if (!!endKey) {
        endIndex_ = FindResult(*results_, endKey);
        if (endIndex_ & FindMissedFlag) {
            endIndex_ = ~endIndex_;
        } else if (!descending_ && inclusiveEnd_) {
            ++endIndex_;
        }
    }
}

DocumentsCollection::size_type MapReduceResults::Offset() const {
    if (!descending_) {
        return startIndex_ + skip_;
    } else {
        // TODO: this is possibly wrong
        return Subtract(results_->size(), 1) - endIndex_ + skip_;
    }
}

DocumentsCollection::size_type MapReduceResults::TotalRows() const {
    return results_->size();
}

DocumentsCollection::size_type MapReduceResults::Limit() const {
    return limit_;
}

DocumentsCollection::size_type MapReduceResults::Skip() const {
    return skip_;
}

DocumentsCollection::size_type MapReduceResults::FindResult(const map_reduce_result_array& results, const map_reduce_query_key_ptr key) {
    const auto size = results.size();
    
    if (size == 0) {
        return ~0;
    } else {
        DocumentsCollection::size_type min = 0;
        DocumentsCollection::size_type mid = 0;
        DocumentsCollection::size_type max = size - 1;
        auto keyId = key->getId();

        while (min <= max) {
            mid = ((max - min) / 2) + min;
            
            auto result = results[mid];
            auto diff = MapReduceResultComparers::CompareField(0, key->GetKeyArray(), result->getResultArray());
            if (diff == 0) {                
                if (keyId && keyId[0] != '\0') {
                    diff = std::strcmp(keyId, result->getId());
                }
            }

            if (diff == 0) {
                return mid;
            } else if (diff < 0) {
                if (mid == 0) {
                    break;
                }
                
                max = mid - 1;
            } else {
                min = mid + 1;
            }
        }

        return ~min;
    }
}

MapReduceResultsIterator MapReduceResults::Iterator() const {
    if (!descending_) {
        auto startIndex = std::min(startIndex_ + skip_, results_->size());
        auto endIndex = std::min(endIndex_, results_->size());
        
        MapReduceResultsIterator iter{results_, startIndex, endIndex, limit_, inclusiveEnd_, descending_};
        return iter;
    } else {
        // to prevent unsigned underflow we have to be careful with the skip calculation
        auto limit = limit_;
        auto endIndex = endIndex_;
        auto diff = endIndex_ - startIndex_ + 1;
        if (skip_ < diff) {
            endIndex = Subtract(endIndex_, skip_);
        } else {
            limit = 0;
        }
        
        MapReduceResultsIterator iter{results_, startIndex_, endIndex, limit, inclusiveEnd_, descending_};
        return iter;
    }
}

DocumentsCollection::size_type MapReduceResults::Subtract(DocumentsCollection::size_type a, DocumentsCollection::size_type b) {
    auto v = a - b;
    if (v > a) {
        v = 0;
    }
    return v;
}