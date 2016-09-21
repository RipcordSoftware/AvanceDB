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

#include "document_collection.h"

DocumentCollection::DocumentCollection(unsigned maxUnsortedEntries, unsigned maxNurseryEntries) : coll_(maxUnsortedEntries, maxNurseryEntries) {
    
}

document_collection_ptr DocumentCollection::Create(unsigned maxUnsortedEntries, unsigned maxNurseryEntries) {
    return boost::make_shared<DocumentCollection>(maxUnsortedEntries, maxNurseryEntries);
}

DocumentCollection::collection::const_iterator DocumentCollection::cbegin() {
    return coll_.cbegin();
}

DocumentCollection::collection::const_iterator DocumentCollection::cend() {
    return coll_.cend();
}

DocumentCollection::const_reference DocumentCollection::operator[](int index) const {
    return coll_[index];
}

void DocumentCollection::lock() const { 
    mtx_.lock(); 
}

bool DocumentCollection::try_lock() const { 
    return mtx_.try_lock(); 
}

void DocumentCollection::unlock() const { 
    mtx_.unlock(); 
}

DocumentCollection::size_type DocumentCollection::size() const {
    return coll_.size();
}

void DocumentCollection::insert(const collection::value_type& k, insert_hint hint) {
    return coll_.insert(k, hint);
}

DocumentCollection::size_type DocumentCollection::erase(const collection::value_type& k) {
    return coll_.erase(k);
}

DocumentCollection::collection::value_type_ptr DocumentCollection::find_fn(collection::compare_type compare) {
    return coll_.find_fn(compare);
}

void DocumentCollection::copy(std::vector<collection::value_type>& coll, bool sort) {
    coll_.copy(coll, sort);
}