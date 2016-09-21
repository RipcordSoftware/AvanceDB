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

#ifndef RS_AVANCEDB_DOCUMENT_REVISION_H
#define RS_AVANCEDB_DOCUMENT_REVISION_H

#include <array>

class DocumentRevision {
public:    
    static const unsigned digestLength_ = 16;
    static const unsigned digestStringLength_ = digestLength_ * 2;
    static const unsigned maxRevStringLength_ = 20 + 1 + digestStringLength_;
    
    using version_type = uint64_t;
    using RevString = std::array<char, maxRevStringLength_ + 1>;
    using Digest = unsigned char[digestLength_];    
    
    DocumentRevision& Increment();
    
    static DocumentRevision Parse(const char*);
    static bool Validate(const char*, bool throwOnFail = false);
    
    DocumentRevision& FormatRevision(RevString& rev);
    static void FormatRevision(version_type version, const Digest& digest, RevString& rev);
    
    version_type getVersion() const;
    void getDigest(Digest&) const;
    
private:    
        
    DocumentRevision(uint64_t version, const Digest& digest);
    
    static unsigned char GetCharNumericValue(char);
    
    version_type version_;
    Digest digest_;
};

#endif /* RS_AVANCEDB_DOCUMENT_REVISION_H */
