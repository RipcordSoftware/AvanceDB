#ifndef JSON_STREAM_H
#define	JSON_STREAM_H

#include <sstream>
#include <string>

class JsonStream final {
public:
    JsonStream();
    
    void Append(const char* name, const std::string& value);
    void Append(const char* name, const char* value);
    void Append(const char* name);
    
    template <typename T> void Append(const char* name, T value) {
        stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
        count_++;
    }
    
    std::string Flush();
    
private:
    std::stringstream stream_;
    int count_;
};

#endif	/* JSON_STREAM_H */