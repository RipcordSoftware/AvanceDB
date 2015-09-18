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

#ifndef SCRIPT_OBJECT_RESPONSE_STREAM_H
#define SCRIPT_OBJECT_RESPONSE_STREAM_H

#include <cstring>
#include <type_traits>
#include <string>

#include "libhttpserver.h"

#include "types.h"

template <unsigned SIZE = 2048>
class ScriptObjectResponseStream final {
public:
    ScriptObjectResponseStream(rs::httpserver::Stream& stream) : 
        pos_(0), stream_(stream) {}
    
    template <typename T>    
    void Serialize(T obj, typename std::enable_if<std::is_same<T, script_object_ptr>::value>::type* = nullptr) {
        AppendObject(obj, false);
    }

    template <typename T>    
    void Serialize(T arr, unsigned index, typename std::enable_if<std::is_same<T, script_array_ptr>::value>::type* = nullptr) {
        auto type = index < arr->getCount() ? arr->getType(index) : rs::scriptobject::ScriptObjectType::Unknown;
        switch (type) {
            case rs::scriptobject::ScriptObjectType::Array: Serialize(arr->getArray(index)); break;
            case rs::scriptobject::ScriptObjectType::Boolean: Serialize(arr->getBoolean(index)); break;
            case rs::scriptobject::ScriptObjectType::Double: Serialize(arr->getDouble(index)); break;
            case rs::scriptobject::ScriptObjectType::Int32: Serialize(arr->getInt32(index)); break;
            case rs::scriptobject::ScriptObjectType::Null: Serialize("null"); break;
            case rs::scriptobject::ScriptObjectType::Object: Serialize(arr->getObject(index)); break;
            case rs::scriptobject::ScriptObjectType::String: AppendString(arr->getString(index)); break;
            default: Serialize("undefined"); break;
        }
    }
    
    template <typename T>
    void Serialize(T arr, typename std::enable_if<std::is_same<T, script_array_ptr>::value>::type* = nullptr) {
        AppendArray(arr, false);
    }
    
    template <typename T>
    void Serialize(T str, typename std::enable_if<std::is_same<T, const char*>::value>::type* = nullptr) {
        AppendLiteralString(str);        
    }
    
    template <typename T>
    void Serialize(T value, bool comma = false, typename std::enable_if<!std::is_same<T, bool>::value && !std::is_same<T, char>::value && std::is_integral<T>::value && std::is_signed<T>::value>::type* = nullptr) {
        AppendInt64(value, comma);
    }
    
    template <typename T>
    void Serialize(T value, bool comma = false, typename std::enable_if<!std::is_same<T, bool>::value && !std::is_same<T, char>::value && std::is_integral<T>::value && std::is_unsigned<T>::value>::type* = nullptr) {
        AppendUInt64(value, comma);
    }    

    template <typename T>
    void Serialize(T value, bool comma = false, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr) {
        AppendDouble(value, comma);
    }

    template <typename T>
    void Serialize(T value, bool comma = false, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr) {
        AppendBool(value, comma);
    }
    
    template <typename T>
    void Serialize(T ch, typename std::enable_if<std::is_same<T, char>::value>::type* = nullptr) {
        char chars[2] = { ch, 0 };
        AppendLiteralString(&chars[0]);
    }
    
    void Flush() {
        FlushBuffer();
        stream_.Flush();
    }
    
    template <typename T>
    friend ScriptObjectResponseStream<SIZE>& operator<<(ScriptObjectResponseStream<SIZE>& stream, T value) {
        stream.Serialize(value);
        return stream;
    }
    
private:

    void AppendObject(script_object_ptr obj, bool comma = false) {
        if (comma) {
            if (getRemainingBytes() == 0) {
                FlushBuffer();
            }

            buffer_[pos_++] = ',';
        }
        
        if (getRemainingBytes() == 0) {
            FlushBuffer();
        }
        
        buffer_[pos_++] = '{';       
        
        auto count = obj->getCount();
        for (decltype(count) i = 0; i < count; ++i) {
            auto name = obj->getName(i);
            AppendName(name, i > 0);
            
            auto type = obj->getType(i);
            switch (type) {
                case rs::scriptobject::ScriptObjectType::Array: AppendArray(obj->getArray(i)); break;
                case rs::scriptobject::ScriptObjectType::Boolean: AppendBool(obj->getBoolean(i)); break;
                case rs::scriptobject::ScriptObjectType::Double: AppendDouble(obj->getDouble(i)); break;
                case rs::scriptobject::ScriptObjectType::Int32: AppendInt64(obj->getInt32(i)); break;
                case rs::scriptobject::ScriptObjectType::Null: AppendNull(); break;
                case rs::scriptobject::ScriptObjectType::Object: AppendObject(obj->getObject(i)); break;
                case rs::scriptobject::ScriptObjectType::String: AppendString(obj->getString(i)); break;
            }
        }
        
        if (getRemainingBytes() == 0) {
            FlushBuffer();
        }
        
        buffer_[pos_++] = '}';
    }

    void AppendArray(script_array_ptr arr, bool comma = false) {
        if (comma) {
            if (getRemainingBytes() == 0) {
                FlushBuffer();
            }

            buffer_[pos_++] = ',';
        }
        
        if (getRemainingBytes() == 0) {
            FlushBuffer();
        }
        
        buffer_[pos_++] = '[';       
        
        auto count = arr->getCount();
        for (decltype(count) i = 0; i < count; ++i) {
            auto type = arr->getType(i);
            switch (type) {
                case rs::scriptobject::ScriptObjectType::Array: AppendArray(arr->getArray(i), i > 0); break;
                case rs::scriptobject::ScriptObjectType::Boolean: AppendBool(arr->getBoolean(i), i > 0); break;
                case rs::scriptobject::ScriptObjectType::Double: AppendDouble(arr->getDouble(i), i > 0); break;
                case rs::scriptobject::ScriptObjectType::Int32: AppendInt64(arr->getInt32(i), i > 0); break;
                case rs::scriptobject::ScriptObjectType::Null: AppendNull(i > 0); break;
                case rs::scriptobject::ScriptObjectType::Object: AppendObject(arr->getObject(i), i > 0); break;
                case rs::scriptobject::ScriptObjectType::String: { auto str = arr->getString(i); AppendString(str, i > 0); break; }
            }
        }
        
        if (getRemainingBytes() == 0) {
            FlushBuffer();
        }
        
        buffer_[pos_++] = ']';
    }

    void AppendName(const char* name, bool comma) {
        AppendString(name, comma);
                        
        if (getRemainingBytes() == 0) {
            FlushBuffer();
        }
        
        buffer_[pos_++] = ':';
    }
    
    void AppendString(const char* value, bool comma = false) {
        std::array<char, 1024> buffer = { { '\0' } };
        std::string dynBuffer;
        value = EscapeJsonString(value, buffer, dynBuffer);
                
        auto strlen = std::strlen(value);
        auto len = strlen + 2 + (comma ? 1: 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        buffer_[pos_++] = '"';
                
        if (len <= getRemainingBytes()) {            
            std::memcpy(buffer_ + pos_, value, strlen);
            pos_ += strlen;
        } else {
            auto tempSize = std::min(getRemainingBytes(), (unsigned)strlen);
            std::memcpy(buffer_ + pos_, value, tempSize);
            pos_ += tempSize;
            FlushBuffer();
            stream_.Write(reinterpret_cast<const rs::httpserver::Stream::byte*>(value), tempSize, strlen - tempSize);
        }
        
        buffer_[pos_++] = '"';
    }
    
    template <typename T, std::size_t N>
    const T* EscapeJsonString(const T* value, std::array<T, N>& buffer, std::basic_string<T>& dynBuffer) {
        std::size_t index = 0;
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
            }
            
            for (; index < len; ++index) {
                switch (value[index]) {
                    case '\\': dynBuffer += "\\\\"; break;
                    case '"': dynBuffer += "\""; break;
                    case '/': dynBuffer += "\\/"; break;
                    case '\b': dynBuffer += "\\b"; break;
                    case '\f': dynBuffer += "\\f"; break;
                    case '\n': dynBuffer += "\\n"; break;
                    case '\r': dynBuffer += "\\r"; break;
                    case '\t': dynBuffer += "\\t"; break;
                    default:  dynBuffer += value[index]; break;
                }
            }
            
            return dynBuffer.c_str();
        }
    }
    
    void AppendLiteralString(const char* str) {
        auto len = std::strlen(str);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
                
        if (len <= getRemainingBytes()) {            
            std::memcpy(buffer_ + pos_, str, len);
            pos_ += len;            
        } else {
            stream_.Write(reinterpret_cast<const rs::httpserver::Stream::byte*>(str), 0, len);
        }
    }
    
    void AppendBool(bool value, bool comma = false) {
        auto len = 5 + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        if (value) {
            buffer_[pos_++] = 't';
            buffer_[pos_++] = 'r';
            buffer_[pos_++] = 'u';
            buffer_[pos_++] = 'e';
        } else {
            buffer_[pos_++] = 'f';
            buffer_[pos_++] = 'a';
            buffer_[pos_++] = 'l';
            buffer_[pos_++] = 's';
            buffer_[pos_++] = 'e';
        }
    }
    
    void AppendNull(bool comma = false) {
        auto len = 4 + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        buffer_[pos_++] = 'n';
        buffer_[pos_++] = 'u';
        buffer_[pos_++] = 'l';
        buffer_[pos_++] = 'l';
    }
    
    void AppendUInt64(std::uint64_t value, bool comma = false) {
        auto len = 20 + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        pos_ += std::sprintf(reinterpret_cast<char*>(buffer_ + pos_), "%llu", value);
    }
    
    void AppendInt64(std::int64_t value, bool comma = false) {
        auto len = 20 + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        pos_ += std::sprintf(reinterpret_cast<char*>(buffer_ + pos_), "%lld", value);
    }
    
    // TODO: review
    void AppendDouble(double value, bool comma = false) {
        char buffer[64];
        auto numLen = std::snprintf(reinterpret_cast<char*>(buffer), sizeof(buffer), "%g", value);
        
        auto len = numLen + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        std::strncpy(reinterpret_cast<char*>(buffer_ + pos_), buffer, numLen);
        pos_ += numLen;
    }
    
    void FlushBuffer() { if (pos_ > 0) { stream_.Write(buffer_, 0, pos_); pos_ = 0; } }

    unsigned getRemainingBytes() { return SIZE - pos_; }    
    
    rs::httpserver::Stream::byte buffer_[SIZE];
    unsigned pos_;
    rs::httpserver::Stream& stream_;
};

#endif	/* SCRIPT_OBJECT_RESPONSE_STREAM_H */

