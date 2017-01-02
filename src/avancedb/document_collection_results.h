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

#ifndef RS_AVANCEDB_DOCUMENT_COLLECTION_RESULTS_H
#define RS_AVANCEDB_DOCUMENT_COLLECTION_RESULTS_H

#include <limits>

#include "types.h"
#include "document_collection.h"

class DocumentCollectionResults final {
public:
    using const_iterator = DocumentCollection::const_iterator;
    using size_type = DocumentCollection::size_type;
    
    DocumentCollectionResults(document_collection_ptr docs, size_type limit, const char* key, const char* startKey, const char* endKey, bool inclusiveEnd, bool descending);
    
    size_type Offset() const;
    size_type FilteredRows() const;
    size_type TotalRows() const;    
    
    const_iterator cbegin() const;
    const_iterator cend() const;
    
private:
    
    const size_type FindMissedFlag = ~(std::numeric_limits<size_type>::max() / 2);
    
    static size_type FindDocument(const DocumentCollection& docs, const char* key);
    
    static size_type Subtract(size_type, size_type);
    
    const document_collection_ptr docs_;
    const bool inclusiveEnd_;
    const bool descending_;
    size_type startIndex_;
    size_type endIndex_;
    const size_type limit_;
};

#endif /* RS_AVANCEDB_DOCUMENT_COLLECTION_RESULTS_H */

