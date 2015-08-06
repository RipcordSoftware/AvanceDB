#include "get_all_documents_options.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "rest_exceptions.h"

GetAllDocumentsOptions::GetAllDocumentsOptions(const rs::httpserver::QueryString& qs) : qs_(qs) {
    
}

bool GetAllDocumentsOptions::HasKey() const {
    return Key().size() > 0;
}

bool GetAllDocumentsOptions::HasKeys() const {
    return StartKey().size() > 0 || EndKey().size() > 0;
}

const std::string& GetAllDocumentsOptions::Key() const {
    if (key_.size() == 0) {
        key_ = GetString("key");
    }
    return key_;
}

const std::string& GetAllDocumentsOptions::StartKey() const {
    if (startKey_.size() == 0) {
        startKey_ = GetString("startkey", "start_key");
    }
    return startKey_;
}

const std::string& GetAllDocumentsOptions::StartKeyDocId() const {
    if (startKeyDocId_.size() == 0) {
        startKeyDocId_ = GetString("startkey_docid", "start_key_doc_id");
    }
    return startKeyDocId_;
}

const std::string& GetAllDocumentsOptions::EndKey() const {
    if (endKey_.size() == 0) {
        endKey_ = GetString("endkey", "end_key" );
    }
    return endKey_;
}

const std::string& GetAllDocumentsOptions::EndKeyDocId() const {
    if (endKeyDocId_.size() == 0) {
        endKeyDocId_ = GetString("endkey_docid", "end_key_doc_id");
    }
    return endKeyDocId_;
}

bool GetAllDocumentsOptions::Descending() const {
    if (!descending_.is_initialized()) {
        descending_ = GetBoolean("descending", false);
    }
    return descending_.get();
}

bool GetAllDocumentsOptions::IncludeDocs() const {
    if (!includeDocs_.is_initialized()) {
        includeDocs_ = GetBoolean("include_docs", false);
    }
    return includeDocs_.get();
}

bool GetAllDocumentsOptions::InclusiveEnd() const {
    if (!inclusiveEnd_.is_initialized()) {
        inclusiveEnd_ = GetBoolean("inclusive_end", true);
    }
    return inclusiveEnd_.get();
}

bool GetAllDocumentsOptions::UpdateSequence() const {
    if (!updateSequence_.is_initialized()) {
        updateSequence_ = GetBoolean("update_seq", false);
    }
    return updateSequence_.get();
}

uint64_t GetAllDocumentsOptions::Skip() const {
    if (!skip_.is_initialized()) {
        skip_ = GetUnsigned("skip", 0);
    }
    return skip_.get();
}

uint64_t GetAllDocumentsOptions::Limit() const {
    if (!limit_.is_initialized()) {
        limit_ = GetUnsigned("limit", std::numeric_limits<uint64_t>::max());
    }
    return limit_.get();
}

bool GetAllDocumentsOptions::GetBoolean(const char* name, bool defaultValue) const {
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

uint64_t GetAllDocumentsOptions::GetUnsigned(const char* name, uint64_t defaultValue) const {
    auto option = std::min(defaultValue, (uint64_t)std::numeric_limits<int64_t>::max());
    
    if (qs_.IsKey(name)) {
        auto value = qs_.getValue(name);    
        if (value.size() > 0) {
            try {            
                option = boost::lexical_cast<uint64_t>(value);
            } catch (const boost::bad_lexical_cast&) {
                throw QueryParseError{"uint64_t", value};
            }
        }
    }
    
    return option;
}

std::string GetAllDocumentsOptions::GetString(const char* name, const char* altName) const {
    if (!!altName) {
        name = qs_.IsKey(altName) ? altName : name;
    }
    
    auto value = qs_.getValue(name);
    if (value.size() > 1 && value.front() == '"' && value.back() == '"') {
        value = std::string{value.cbegin() + 1, value.cend() - 1};
    }
    
    return value;
}