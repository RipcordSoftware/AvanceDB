#include "json_stream.h"

JsonStream::JsonStream(ContextType contextType) : 
        contextType_(contextType), fieldCount_(0) {
    EmitOpenContext();
}

void JsonStream::EmitFieldDelimiters() {
    if (fieldCount_ > 0) {
        stream_ << ',';
    }
    
    ++fieldCount_;
}

std::string JsonStream::Flush() {
    while (PopContext());
    
    EmitCloseContext();
    
    auto str = stream_.str();
    stream_.str("");
    fieldCount_ = 0;
    return std::move(str);
}

void JsonStream::PushContext(ContextType contextType, const char* name) {
    if (fieldCount_ > 0) {
        stream_ << ",";
    }
    
    contextStack_.emplace(contextType_, fieldCount_ + 1);
    contextType_ = contextType;
    fieldCount_ = 0;
    
    if (name) {
        stream_ << '"' << name << "\":";
    }
    
    EmitOpenContext();
}

bool JsonStream::PopContext() {
    auto status = contextStack_.size() > 0;
    
    if (status) {
        EmitCloseContext();
        
        auto context = contextStack_.top();
        contextStack_.pop();
        
        contextType_ = context.type_;
        fieldCount_ = context.fieldCount_;
    }
    
    return status;
}

void JsonStream::EmitOpenContext() {
    switch (contextType_) {
        case ContextType::Object: stream_ << "{"; break;
        case ContextType::Array: stream_ << "["; break;
    }
}

void JsonStream::EmitCloseContext() {
    switch (contextType_) {
        case ContextType::Object: stream_ << "}"; break;
        case ContextType::Array: stream_ << "]"; break;
    }
}