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

#ifndef RS_AVANCEDB_DOCUMENT_COLLECTION_H
#define RS_AVANCEDB_DOCUMENT_COLLECTION_H

#include "types.h"
#include "document.h"

#include <vector>

#include <boost/thread.hpp>
#include <boost/make_shared.hpp>

#include "../../externals/lazyflatset/lazyflatset.hpp"

class DocumentCollection final {
public:
    using collection = rs::LazyFlatSet<document_ptr, Document::Less, Document::Equal, rs::LazyFlatSetQuickSort<document_ptr, Document::Less>, std::allocator<document_ptr>, true>;
    using const_iterator = collection::const_iterator;
    using const_reference = collection::const_reference;
    using size_type = collection::size_type;
    using insert_hint = collection::insert_hint;
    
    static document_collection_ptr Create(unsigned maxUnsortedEntries = 16, unsigned maxNurseryEntries = 1024);
    
    collection::const_iterator cbegin();
    collection::const_iterator cend();
    
    const_reference operator[](int index) const;
    
    void lock() const;
    bool try_lock() const;
    void unlock() const;
    
    size_type size() const;
    void insert(const collection::value_type&, insert_hint hint = insert_hint::no_hint);
    size_type erase(const collection::value_type&);
    void copy(std::vector<collection::value_type>& coll, bool sort = true);
    collection::value_type_ptr find_fn(collection::compare_type);
    
private:
    
    friend document_collection_ptr boost::make_shared<document_collection_ptr::element_type>(unsigned&, unsigned&);
    
    DocumentCollection(unsigned maxUnsortedEntries, unsigned maxNurseryEntries);
    
    collection coll_;
    
    mutable boost::mutex mtx_;
    char padding_[64];
};

#endif /* RS_AVANCEDB_DOCUMENT_COLLECTION_H */

