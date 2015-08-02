#ifndef SCRIPT_OBJECT_RESPONSE_STREAM_H
#define	SCRIPT_OBJECT_RESPONSE_STREAM_H

#include <cstring>

#include "libhttpserver.h"

#include "types.h"

template <unsigned SIZE = 2048>
class ScriptObjectResponseStream final {
public:
    ScriptObjectResponseStream(rs::httpserver::Stream& stream) : 
        pos_(0), stream_(stream) {}
        
    void Serialize(script_object_ptr obj) {
        AppendObject(obj, false);
    }
    
    void Serialize(const char* str) {
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
    
    void Serialize(int value, bool comma = false) {
        AppendInt32(value, comma);
    }
    
    void Serialize(std::uint64_t value, bool comma = false) {
        AppendUInt64(value, comma);
    }        
    
    void Flush() {
        FlushBuffer();
        stream_.Flush();
    }
    
    friend ScriptObjectResponseStream<SIZE>& operator<<(ScriptObjectResponseStream<SIZE>& stream, script_object_ptr obj) {
        stream.Serialize(obj);
        return stream;
    }
    
    friend ScriptObjectResponseStream<SIZE>& operator<<(ScriptObjectResponseStream<SIZE>& stream, const char* str) {
        stream.Serialize(str);
        return stream;
    }
    
    friend ScriptObjectResponseStream<SIZE>& operator<<(ScriptObjectResponseStream<SIZE>& stream, char ch) {
        char chars[2] = { ch, 0 };
        stream.Serialize(&chars[0]);
        return stream;
    }
    
    friend ScriptObjectResponseStream<SIZE>& operator<<(ScriptObjectResponseStream<SIZE>& stream, int value) {
        stream.Serialize(value);
        return stream;
    }
    
    friend ScriptObjectResponseStream<SIZE>& operator<<(ScriptObjectResponseStream<SIZE>& stream, std::uint64_t value) {
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
                case rs::scriptobject::ScriptObjectType::Int32: AppendInt32(obj->getInt32(i)); break;
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
                case rs::scriptobject::ScriptObjectType::Int32: AppendInt32(arr->getInt32(i), i > 0); break;
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

    bool AppendName(const char* name, bool comma) {
        AppendString(name, comma);
                        
        if (getRemainingBytes() == 0) {
            FlushBuffer();
        }
        
        buffer_[pos_++] = ':';
    }
    
    bool AppendString(const char* value, bool comma = false) {        
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
    
    bool AppendBool(bool value, bool comma = false) {
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
    
    bool AppendNull(bool comma = false) {
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
    
    bool AppendInt32(int value, bool comma = false) {
        auto len = 10 + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        pos_ += std::sprintf(reinterpret_cast<char*>(buffer_ + pos_), "%d", value);
    }
    
    bool AppendUInt64(std::uint64_t value, bool comma = false) {
        auto len = 20 + (comma ? 1 : 0);
        
        if (len > getRemainingBytes()) {
            FlushBuffer();
        }
        
        if (comma) {
            buffer_[pos_++] = ',';
        }
        
        pos_ += std::sprintf(reinterpret_cast<char*>(buffer_ + pos_), "%lu", value);
    }
    
    // TODO: review
    bool AppendDouble(double value, bool comma = false) {
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

