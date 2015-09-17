/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015 Ripcord Software
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