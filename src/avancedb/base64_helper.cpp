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

#include "base64_helper.h"

#include "../../externals/ConstTimeEncoding/base64.h"

std::string Base64Helper::Encode(const buffer_type& data) {
    auto encodedSize = data.size() * 4;
    encodedSize = (encodedSize + 3) / 3;
    encodedSize += 4 - (encodedSize % 4);   
    
    std::string text;
    text.resize(encodedSize);
    
    base64Encode(&text[0], data.data(), data.size());    
    
    return text;
}

Base64Helper::buffer_type Base64Helper::Decode(const char* text, std::size_t size) {
    buffer_type data;
    
    if (size > 0) {        
        auto decodedSize = GetDecodedSize(text, size);
        data.resize(decodedSize);

        base64Decode(data.data(), text, size);
    }
    
    return data;
}

std::size_t Base64Helper::GetDecodedSize(const char* text, std::size_t size) {
    std::size_t decodedSize = 0;
    
    if (size > 0) {
        decodedSize = size;
        while (text[decodedSize - 1] == '=') {
            --decodedSize;
        }
        
        decodedSize *= 3;
        decodedSize /= 4;
    }
    
    return decodedSize;
}