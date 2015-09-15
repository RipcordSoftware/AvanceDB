#ifndef JSON_STREAM_H
#define	JSON_STREAM_H

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

#endif	/* JSON_STREAM_H */