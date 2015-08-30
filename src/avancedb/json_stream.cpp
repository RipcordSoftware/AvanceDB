#include "json_stream.h"

JsonStream::JsonStream() : count_(0) {
    stream_ << "{";
}

void JsonStream::Append(const char* name, const std::string& value) {
    stream_ << (count_ > 0 ? "," : "") << "\"" << name << "\":\"" << value << "\"";
    count_++;
}

void JsonStream::Append(const char* name, const char* value) {
    stream_ << (count_ > 0 ? "," : "") << "\"" << name << "\":\"" << value << "\"";
    count_++;
}

void JsonStream::Append(const char* name) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << "null";
    count_++;
}

std::string JsonStream::Flush() {
    stream_ << "}";
    
    auto str = stream_.str();
    stream_.str("");
    return std::move(str);
}