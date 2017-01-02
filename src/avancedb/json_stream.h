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

#ifndef RS_AVANCEDB_JSON_STREAM_H
#define RS_AVANCEDB_JSON_STREAM_H

#include <sstream>
#include <string>
#include <type_traits>
#include <stack>

class JsonStream final {
public:
    enum class ContextType { Object, Array };
    
    JsonStream(ContextType contextType = ContextType::Object);

    template <typename T>
    void Append(const char* name, T value, typename std::enable_if<!std::is_same<T, bool>::value && std::is_arithmetic<T>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << "\"" << name << "\":" << value;        
    }   
    
    template <typename T>
    void Append(const char* name, T value, typename std::enable_if<std::is_same<T, const char*>::value || std::is_same<T, char*>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << "\"" << name << "\":\"" << value << "\"";
    }
    
    template <typename T>
    void Append(const char* name, const T& value, typename std::enable_if<std::is_same<T, std::string>::value>::type* = nullptr) {
        Append(name, value.c_str());
    }
    
    template <typename T>
    void Append(const char* name, T value, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << "\"" << name << "\":" << (value ? "true" : "false");
    }
    
    template <typename T>
    void Append(T value, typename std::enable_if<!std::is_same<T, bool>::value && std::is_arithmetic<T>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << value;
    }
    
    template <typename T>
    void Append(T value, typename std::enable_if<std::is_same<T, const char*>::value || std::is_same<T, char*>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << '"' << value << '"';
    }
    
    template <typename T>
    void Append(T value, typename std::enable_if<std::is_same<T, std::string>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << '"' << value << '"';
    }
    
    template <typename T>
    void Append(T value, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr) {
        EmitFieldDelimiters();
        stream_ << (value ? "true" : "false");
    }
    
    void PushContext(ContextType contextType, const char* name = nullptr);
    bool PopContext();

    std::string Flush();
    
private:
    struct Context final {
        Context(ContextType type, int fieldCount) : 
            type_(type), fieldCount_(fieldCount) {}
        
        const ContextType type_;
        const int fieldCount_;
    };
    
    void EmitOpenContext();
    void EmitCloseContext();    
    void EmitFieldDelimiters();
    
    std::stringstream stream_;
        
    ContextType contextType_;
    int fieldCount_;
    
    std::stack<Context> contextStack_;
};

#endif /* RS_AVANCEDB_JSON_STREAM_H */