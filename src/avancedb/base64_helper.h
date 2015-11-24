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

#ifndef BASE64_HELPER_H
#define BASE64_HELPER_H

#include <string>
#include <vector>

class Base64Helper final {
public:
    Base64Helper() = delete;
    
    static std::string Encode(const std::vector<unsigned char>& data);
    static std::vector<unsigned char> Decode(const char* text, std::size_t size);
    
private:

};

#endif	/* BASE64_HELPER_H */

