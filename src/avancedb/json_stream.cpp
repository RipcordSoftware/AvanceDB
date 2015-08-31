#include "json_stream.h"

JsonStream::JsonStream() : count_(0) {
    stream_ << "{";
}

std::string JsonStream::Flush() {
    stream_ << "}";
    
    auto str = stream_.str();
    stream_.str("");
    return std::move(str);
}