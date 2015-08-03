#include "get_all_documents_options.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "rest_exceptions.h"

GetAllDocumentsOptions::GetAllDocumentsOptions(const rs::httpserver::QueryString& qs) : qs_(qs) {
    
}

const std::string& GetAllDocumentsOptions::Key() const {
    return qs_.getValue("key");
}

const std::string& GetAllDocumentsOptions::StartKey() const {
    auto name = qs_.IsKey("start_key") ? "start_key" : "startkey";
    return qs_.getValue(name);
}

const std::string& GetAllDocumentsOptions::StartKeyDocId() const {
    auto name = qs_.IsKey("start_key_doc_id") ? "start_key_doc_id" : "startkey_docid";
    return qs_.getValue(name);
}

const std::string& GetAllDocumentsOptions::EndKey() const {
    auto name = qs_.IsKey("end_key") ? "end_key" : "endkey";
    return qs_.getValue(name);
}

const std::string& GetAllDocumentsOptions::EndKeyDocId() const {
    auto name = qs_.IsKey("end_key_doc_id") ? "end_key_doc_id" : "endkey_docid";
    return qs_.getValue(name);
}

bool GetAllDocumentsOptions::Conflicts() const {
    return GetBoolean("conflicts", false);
}

bool GetAllDocumentsOptions::Descending() const {
    return GetBoolean("descending", false);
}

bool GetAllDocumentsOptions::IncludeDocs() const {
    return GetBoolean("include_docs", false);
}

bool GetAllDocumentsOptions::InclusiveEnd() const {
    return GetBoolean("inclusive_end", true);
}

bool GetAllDocumentsOptions::UpdateSequence() const {
    return GetBoolean("update_seq", false);
}

int GetAllDocumentsOptions::Skip() const {
    return GetInteger("skip", 0);
}

int GetAllDocumentsOptions::Limit() const {
    return GetInteger("limit", std::numeric_limits<int>::max());
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

int GetAllDocumentsOptions::GetInteger(const char* name, int defaultValue) const {
    int option = defaultValue;    
    
    if (qs_.IsKey(name)) {
        auto value = qs_.getValue(name);    
        if (value.size() > 0) {
            try {            
                option = boost::lexical_cast<int>(value);
            } catch (const boost::bad_lexical_cast&) {
                throw QueryParseError{"integer", value};
            }
        }
    }
    
    return option;
}