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

#include "map_reduce_result_array.h"

#include <stdexcept>

MapReduceResultArray::MapReduceResultArray(collection::size_type capacity) {
    data_.reserve(capacity);
}

MapReduceResultArray::MapReduceResultArray(MapReduceResultArray&& rhs) : data_(std::move(rhs.data_)) {
    
}

MapReduceResultArray::~MapReduceResultArray() {    
    // TODO: revise this
    // if there are no sources then we own the pointers
    if (sources_.size() == 0) {
        for (auto iter : data_) {
            delete iter;
        }
    }
}

MapReduceResultArray::size_type MapReduceResultArray::capacity() const {
    return data_.capacity();
}

void MapReduceResultArray::reserve(size_type n) {
    data_.reserve(n);
}

void MapReduceResultArray::resize(size_type n) {
    data_.resize(n);
}

MapReduceResultArray::size_type MapReduceResultArray::size() const {
    return data_.size();
}

MapReduceResultArray::const_iterator MapReduceResultArray::cbegin() const {
    return data_.cbegin();
}

MapReduceResultArray::const_iterator MapReduceResultArray::cend() const {
    return data_.cend();
}

MapReduceResultArray::iterator MapReduceResultArray::begin() {
    return data_.begin();
}

MapReduceResultArray::iterator MapReduceResultArray::end() {
    return data_.end();
}

void MapReduceResultArray::push_back(map_reduce_result_ptr ptr) {
    data_.push_back(ptr);
}

void MapReduceResultArray::insert(iterator position, const_iterator first, const_iterator last, const map_reduce_result_array_ptr& sourcePtr) {
    // TODO: this case has two personalities - that should be refactored out sometime
    if (data_.size() > 0 && sources_.size() == 0) {
        throw std::logic_error{"Unable to insert - mixed pointer ownership is not supported"};
    }
    
    data_.insert(position, first, last);
    sources_.push_back(sourcePtr);
}

MapReduceResultArray::const_reference MapReduceResultArray::operator[](int n) const {
    return data_[n];
}

void MapReduceResultArray::clear() {
    data_.clear();
}