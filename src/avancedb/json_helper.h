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

#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <array>
#include <string>
#include <cstring>

class JsonHelper final {
public:    
    JsonHelper() = delete;

    template <typename T, std::size_t N>
    static const T* EscapeJsonString(const T* value, std::array<T, N>& buffer, std::basic_string<T>& dynBuffer) {
        std::size_t index = 0;
        
        buffer[0] = '\0';
        dynBuffer.clear();
        
        auto len = std::strlen(value);
        if (len > 0 && len < N) {
            decltype(index) i = 0;
            for (; index < len && i < N - 2; ++index) {
                switch (value[index]) {
                    case '\\': buffer[i++] = '\\'; buffer[i++] = '\\'; break;
                    case '"': buffer[i++] = '\\'; buffer[i++] = '"'; break;
                    case '/': buffer[i++] = '\\'; buffer[i++] = '/'; break;
                    case '\b': buffer[i++] = '\\'; buffer[i++] = 'b'; break;
                    case '\f': buffer[i++] = '\\'; buffer[i++] = 'f'; break;
                    case '\n': buffer[i++] = '\\'; buffer[i++] = 'n'; break;
                    case '\r': buffer[i++] = '\\'; buffer[i++] = 'r'; break;
                    case '\t': buffer[i++] = '\\'; buffer[i++] = 't'; break;
                    default: buffer[i++] = value[index]; break;
                }
            }
            
            buffer[i] = '\0';
        }
        
        if (index == len) {
            return buffer.data();
        } else {
            if (index > 0) {
                dynBuffer = buffer.data();
                buffer[0] = '\0';
            }
            
            for (; index < len; ++index) {
                EscapeJsonChar(value[index], dynBuffer);
            }
            
            return dynBuffer.c_str();
        }
    }
    
    template <typename T>
    static std::basic_string<T> EscapeJsonString(const T* value) {
        std::basic_string<T> buffer;
        auto len = std::strlen(value);      
            
        for (decltype(len) i = 0; i < len; ++i) {
            EscapeJsonChar(value[i], buffer);
        }

        return buffer;
    }

private:    
    template <typename T>
    static inline void EscapeJsonChar(T ch, std::basic_string<T>& buffer) {
        switch (ch) {
            case '\\': buffer += "\\\\"; break;
            case '"': buffer += "\\\""; break;
            case '/': buffer += "\\/"; break;
            case '\b': buffer += "\\b"; break;
            case '\f': buffer += "\\f"; break;
            case '\n': buffer += "\\n"; break;
            case '\r': buffer += "\\r"; break;
            case '\t': buffer += "\\t"; break;
            default:  buffer += ch; break;
        }
    }    
};


#endif	/* JSON_HELPER_H */

