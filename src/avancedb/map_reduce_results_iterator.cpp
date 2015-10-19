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

MapReduceResultsIterator::MapReduceResultsIterator(map_reduce_result_array_ptr results, 
        DocumentsCollection::size_type startIndex, DocumentsCollection::size_type endIndex, DocumentsCollection::size_type limit, bool inclusiveEnd, bool descending) :
                results_(results), startIndex_(std::min(startIndex, results->size())), 
                endIndex_(std::min(endIndex, results->size())), 
                inclusiveEnd_(inclusiveEnd), descending_(descending), 
                index_(CalculateEndIndex(startIndex_, endIndex_, results_->size(), descending)),
                limit_(limit - 1) {
    
}

map_reduce_result_array_ptr::element_type::const_reference MapReduceResultsIterator::operator++() const {
    ptr_ = nullptr;
            
    if (!descending_) {
        ++index_;
        --limit_;
        if (CheckIndex(index_)) {
            ptr_ = (*results_)[index_];            
        }
    } else {
        --index_;
        --limit_;
        if (CheckIndex(index_)) {
            ptr_ = (*results_)[index_];            
        }
    }
    
    return ptr_; 
}

map_reduce_result_array_ptr::element_type::const_reference MapReduceResultsIterator::operator++(int) const {
    ptr_ = nullptr;
    
    if (!descending_) {
        if (CheckIndex(index_)) {
            ptr_ = (*results_)[index_];
            ++index_;
            --limit_;
        }
    } else {
        if (CheckIndex(index_)) {
            ptr_ = (*results_)[index_];
            --index_;
            --limit_;
        }
    }
    
    return ptr_; 
}

map_reduce_result_array_ptr::element_type::const_reference MapReduceResultsIterator::operator*() const {
    if (!ptr_ && CheckIndex(index_)) {
        ptr_ = (*results_)[index_];
    }
    
    return ptr_;
}

bool MapReduceResultsIterator::CheckIndex(DocumentsCollection::size_type index) const {
    bool check = false;
    
    if (index >= 0 && index < results_->size() && limit_ >= 0) {
        if (!descending_) {
            check = index_ < endIndex_;
        } else {
            check = index_ >= startIndex_;
        }
    }
    
    return check;
}

DocumentsCollection::size_type MapReduceResultsIterator::CalculateEndIndex(DocumentsCollection::size_type startIndex, DocumentsCollection::size_type endIndex, DocumentsCollection::size_type size, bool descending) {
    if (descending) {
        if (size > 0 && endIndex >= size && startIndex < size) {
            return size - 1;
        } else {
            return endIndex;
        }
    } else {
        return startIndex;
    }
}