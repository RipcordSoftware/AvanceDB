#ifndef JSON_STREAM_H
#define	JSON_STREAM_H

#include <sstream>
#include <string>
#include <type_traits>

class JsonStream final {
public:
    JsonStream();

    template <typename T>
    void Append(const char* name, T value, typename std::enable_if<!std::is_same<T, bool>::value && std::is_arithmetic<T>::value>::type* = nullptr) {
        stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
        count_++;
    }   
    
    template <typename T>
    void Append(const char* name, T value, typename std::enable_if<std::is_same<T, const char*>::value || std::is_same<T, char*>::value ||std::is_same<T, const std::string&>::value>::type* = nullptr) {
        stream_ << (count_ > 0 ? "," : "") << "\"" << name << "\":\"" << value << "\"";
        count_++;
    }
    
    template <typename T>
    void Append(const char* name, T value, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr) {
        stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << (value ? "true" : "false");
        count_++;
    }
    
    std::string Flush();
    
private:
    std::stringstream stream_;
    int count_;
};

#endif	/* JSON_STREAM_H */