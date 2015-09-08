#include "json_stream.h"

JsonStream::JsonStream() : fieldCount_(0), objectCount_(0) {   
}

JsonStream::JsonStream(const char* prefix) : JsonStream() {
    stream_ << prefix;
}

void JsonStream::NextObject() {
    if (fieldCount_ > 0) {
        stream_ << '}';
    }

    fieldCount_ = 0;
    ++objectCount_;
}

void JsonStream::EmitFieldDelimiters() {
    if (fieldCount_ > 0) {
        stream_ << ',';
    } else {
        if (objectCount_ > 0) {
            stream_ << ',';
        }
        
        stream_ << '{';
    }
    
    ++fieldCount_;
}

std::string JsonStream::Flush(const char* postfix) {
    if (fieldCount_ > 0) {
        stream_ << "}";
    }
    
    if (postfix) {
        stream_ << postfix;
    }
    
    auto str = stream_.str();
    stream_.str("");
    return std::move(str);
}