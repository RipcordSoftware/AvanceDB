#include "map_reduce_shard_results.h"

#include "map_reduce_results.h"
#include "map_reduce_result_comparers.h"
#include "map_reduce_query_key.h"

#include <algorithm>
#include <cstring>

MapReduceShardResults::MapReduceShardResults(map_reduce_result_array_ptr results,
        size_type limit, map_reduce_query_key_ptr startKey, map_reduce_query_key_ptr endKey, 
        bool inclusiveEnd, bool descending) :
        results_(results), limit_(std::min(limit, results->size())),
        startIndex_(0), endIndex_(results->size()),
        inclusiveEnd_(inclusiveEnd), descending_(descending) {
    
    if (descending_) {
        std::swap(startKey, endKey);
    }
    
    // TODO: scan for next non-matching item
    if (!!startKey) {
        startIndex_ = FindResult(*results_, startKey);
        if (startIndex_ & FindMissedFlag) {
            startIndex_ = ~startIndex_;
        } else {
            auto inclusiveStart = descending_ ? inclusiveEnd_ : true;
            if (!inclusiveStart) {
                ++startIndex_;
            }
        }
    }
    
    // TODO: scan for next non-matching item
    if (!!endKey) {
        endIndex_ = FindResult(*results_, endKey);
        if (endIndex_ & FindMissedFlag) {
            endIndex_ = ~endIndex_;
        } else {
            auto inclusiveEnd = !descending_ ? inclusiveEnd_ : true;
            if (inclusiveEnd) {
                ++endIndex_;
            }
        }
    }    
    
    if (!descending_ && endIndex_ < startIndex_) {
        endIndex_ = startIndex_;
    } else if (descending && startIndex_ > endIndex_) {
        startIndex_ = endIndex_;
    }
}

MapReduceShardResults::size_type MapReduceShardResults::Offset() const {
    if (!descending_) {
        return startIndex_;
    } else {
        return results_->size() - endIndex_;
    }
}

MapReduceShardResults::size_type MapReduceShardResults::FilteredRows() const {
    return std::distance(cbegin(), cend());
}

MapReduceShardResults::size_type MapReduceShardResults::TotalRows() const {
    return results_->size();
}

MapReduceShardResults::size_type MapReduceShardResults::FindResult(const MapReduceResultArray& results, const map_reduce_query_key_ptr key) {
    const auto size = results.size();
    
    if (size == 0) {
        return ~0;
    } else {
        size_type min = 0;
        size_type mid = 0;
        size_type max = size - 1;
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

MapReduceShardResults::const_iterator MapReduceShardResults::cbegin() const {
    if (!descending_) {
        return results_->cbegin() + startIndex_;
    } else {
        auto startIndex = Subtract(endIndex_, limit_);
        startIndex = std::max(startIndex, startIndex_);
        return results_->cbegin() + startIndex;
    }
}

MapReduceShardResults::const_iterator MapReduceShardResults::cend() const {
    if (!descending_) {    
        auto endIndex = std::min(endIndex_, startIndex_ + limit_);
        return results_->cbegin() + endIndex;
    } else {
        return results_->cbegin() + endIndex_;
    }
}

MapReduceShardResults::size_type MapReduceShardResults::Subtract(size_type a, size_type b) {
    auto v = a - b;
    if (v > a) {
        v = 0;
    }
    return v;
}

map_reduce_result_array_ptr MapReduceShardResults::SourceResults() const {
    return results_;
}