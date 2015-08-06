#include "post_all_documents_options.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "rest_exceptions.h"

PostAllDocumentsOptions::PostAllDocumentsOptions(const rs::httpserver::QueryString& qs, script_array_ptr keysArray) : qs_(qs), keysArray_(keysArray) {
    
}

const PostAllDocumentsOptions::keys_array& PostAllDocumentsOptions::Keys() const {
    if (keys_.size() == 0) {
        auto count = keysArray_->getCount();
        
        for (decltype(count) i = 0; i < count; ++i) {
            if (keysArray_->getType(i) == rs::scriptobject::ScriptObjectType::String) {
                auto key = keysArray_->getString(i);
                keys_.emplace_back(key);
            } else {
                keys_.push_back(_emptyString);
            }
        }
    }
    
    return keys_;
}

bool PostAllDocumentsOptions::Descending() const {
    if (!descending_.is_initialized()) {
        descending_ = GetBoolean("descending", false);
    }
    return descending_.get();
}

bool PostAllDocumentsOptions::IncludeDocs() const {
    if (!includeDocs_.is_initialized()) {
        includeDocs_ = GetBoolean("include_docs", false);
    }
    return includeDocs_.get();
}

bool PostAllDocumentsOptions::UpdateSequence() const {
    if (!updateSequence_.is_initialized()) {
        updateSequence_ = GetBoolean("update_seq", false);
    }
    return updateSequence_.get();
}

uint64_t PostAllDocumentsOptions::Skip() const {
    if (!skip_.is_initialized()) {
        skip_ = GetUnsigned("skip", 0);
    }
    return skip_.get();
}

uint64_t PostAllDocumentsOptions::Limit() const {
    if (!limit_.is_initialized()) {
        limit_ = GetUnsigned("limit", std::numeric_limits<uint64_t>::max());
    }
    return limit_.get();
}

bool PostAllDocumentsOptions::GetBoolean(const char* name, bool defaultValue) const {
    bool option = defaultValue;
    
    if (qs_.IsKey(name)) {
        auto value = qs_.getValue(name);
        if (value.size() > 0) {
            if (boost::iequals(value, "true")) {
                option = true;
            } else if (boost::iequals(value, "false")) {
                option = false;
            } else {            
                throw QueryParseError{"boolean", value};
            }
        }
    }
    
    return option;
}

uint64_t PostAllDocumentsOptions::GetUnsigned(const char* name, uint64_t defaultValue) const {
    auto option = std::min(defaultValue, (uint64_t)std::numeric_limits<int64_t>::max());
    
    if (qs_.IsKey(name)) {
        auto value = qs_.getValue(name);    
        if (value.size() > 0) {
            auto valid = false;

            try {
                if (value[0] != '-') {
                    option = boost::lexical_cast<uint64_t>(value);
                    valid = true;
                }
            } catch (const boost::bad_lexical_cast&) {
            }

            if (!valid) {
                throw QueryParseError{"positive integer", value};
            }
        }
    }
    
    return option;
}
