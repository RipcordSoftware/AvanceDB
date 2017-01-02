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

#ifndef RS_AVANCEDB_BASE64_HELPER_H
#define RS_AVANCEDB_BASE64_HELPER_H

#include <string>
#include <vector>

class Base64Helper final {
public:
    using buffer_type = std::vector<unsigned char>;
    
    Base64Helper() = delete;
    
    static std::string Encode(const buffer_type& data);
    static buffer_type Decode(const char* text, std::size_t size);
    
    static std::size_t GetDecodedSize(const char* text, std::size_t size);
    
private:

};

#endif /* RS_AVANCEDB_BASE64_HELPER_H */

