#include "json_stream.h"

JsonStream::JsonStream() : count_(0) {
    stream_ << "{";
}

void JsonStream::Append(const char* name, const char* value) {
    stream_ << (count_ > 0 ? "," : "") << "\"" << name << "\":\"" << value << "\"";
    count_++;
}

void JsonStream::Append(const char* name, int value) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
    count_++;
}

void JsonStream::Append(const char* name, unsigned value) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
    count_++;
}

void JsonStream::Append(const char* name, int64_t value) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
    count_++;
}

void JsonStream::Append(const char* name, uint64_t value) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
    count_++;
}

void JsonStream::Append(const char* name, double value) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << value;
    count_++;
}

void JsonStream::Append(const char* name, bool value) {
    stream_ << (count_ > 0 ? "," : "")  << "\"" << name << "\":" << (value ? "true" : "false");
    count_++;
}

std::string JsonStream::Flush() {
    stream_ << "}";
    
    auto str = stream_.str();
    stream_.str("");
    return std::move(str);
}