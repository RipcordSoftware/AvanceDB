#ifndef JSON_STREAM_H
#define	JSON_STREAM_H

#include <sstream>
#include <string>
#include <type_traits>

class JsonStream final {
public:
    JsonStream();
    JsonStream(const char* prefix);

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
    
    void NextObject();

    std::string Flush(const char* postfix = nullptr);
    
private:
    void EmitFieldDelimiters();
    
    std::stringstream stream_;
    int fieldCount_;
    int objectCount_;
};

#endif	/* JSON_STREAM_H */