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

#include "document_collection_results.h"

#include <cstring>
#include <algorithm>

DocumentCollectionResults::DocumentCollectionResults(document_collection_ptr docs, 
    size_type limit, const char* key,
    const char* startKey, const char* endKey, bool inclusiveEnd, bool descending) :
        docs_(docs),
        limit_(std::min(limit, docs->size())),
        startIndex_(0), endIndex_(docs->size()),
        inclusiveEnd_(inclusiveEnd), descending_(descending) {
    
    if (key && key[0] != '\0') {
        startIndex_ = FindDocument(*docs, key);
        if ((startIndex_ & FindMissedFlag) == FindMissedFlag) {
            startIndex_ = ~startIndex_;
            endIndex_ = startIndex_;
        } else {
            endIndex_ = startIndex_ + 1;
        }
    } else {    
        if (descending_) {
            std::swap(startKey, endKey);
        }

        if (startKey && startKey[0] != '\0') {
            startIndex_ = FindDocument(*docs_, startKey);
            if (startIndex_ & FindMissedFlag) {
                startIndex_ = ~startIndex_;
            } else {
                auto inclusiveStart = descending_ ? inclusiveEnd_ : true;
                if (!inclusiveStart) {
                    ++startIndex_;
                }
            }
        }

        if (endKey && endKey[0] != '\0') {
            endIndex_ = FindDocument(*docs_, endKey);
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
}

DocumentCollectionResults::size_type DocumentCollectionResults::FindDocument(const DocumentCollection& docs, const char* key) {
    const auto size = docs.size();
    
    if (size == 0) {
        return ~0;
    } else {
        const char* keyId = key;
        auto keyIdLength = std::strlen(key);
        if (keyId[0] == '"' && keyId[keyIdLength - 1] == '"') {
            keyId++;
            keyIdLength -= 2;
        }
        
        DocumentCollection::size_type min = 0;
        DocumentCollection::size_type mid = 0;
        DocumentCollection::size_type max = size - 1;

        while (min <= max) {
            mid = ((max - min) / 2) + min;

            auto docId = docs[mid]->getId();
            auto diff = std::strncmp(keyId, docId, keyIdLength);
            if (diff == 0 && docId[keyIdLength] != '\0') {
                diff = -1;
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

DocumentCollectionResults::size_type DocumentCollectionResults::Offset() const {
    if (!descending_) {
        return startIndex_;
    } else {
        return docs_->size() - endIndex_;
    }
}

DocumentCollectionResults::size_type DocumentCollectionResults::FilteredRows() const {
    return std::distance(cbegin(), cend());
}

DocumentCollectionResults::size_type DocumentCollectionResults::TotalRows() const {
    return docs_->size();
}

DocumentCollectionResults::const_iterator DocumentCollectionResults::cbegin() const {
    if (!descending_) {
        return docs_->cbegin() + startIndex_;
    } else {
        auto startIndex = Subtract(endIndex_, limit_);
        startIndex = std::max(startIndex, startIndex_);
        return docs_->cbegin() + startIndex;
    }
}

DocumentCollectionResults::const_iterator DocumentCollectionResults::cend() const {
    if (!descending_) {    
        auto endIndex = std::min(endIndex_, startIndex_ + limit_);
        return docs_->cbegin() + endIndex;
    } else {
        return docs_->cbegin() + endIndex_;
    }
}

DocumentCollectionResults::size_type DocumentCollectionResults::Subtract(size_type a, size_type b) {
    auto v = a - b;
    if (v > a) {
        v = 0;
    }
    return v;
}