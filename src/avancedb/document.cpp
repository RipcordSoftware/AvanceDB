#include "document.h"

#include <cstring>
#include <cstdlib>

#include "script_object_vector_source.h"

Document::Document(script_object_ptr obj) : obj_(obj), id_(obj->getString("_id")), rev_(obj->getString("_rev")) {
}

document_ptr Document::Create(const char* id, script_object_ptr obj) {
    const char* oldRev = obj->getType("_rev") == rs::scriptobject::ScriptObjectType::String ? 
        obj->getString("_rev") : nullptr;
    
    // TODO: strtoul is unsafe
    auto oldVersion = oldRev != nullptr ? std::strtoul(oldRev, nullptr, 10) : 0;
    auto nextVersion = oldVersion + 1;
    
    rs::scriptobject::ScriptObjectHash digest;
    obj->CalculateHash(digest, &Document::ValidateHashField);
    
    RevString rev;
    FormatRevision(nextVersion, digest, rev);
    
    document_ptr doc;
    if (obj->getType("_id") == rs::scriptobject::ScriptObjectType::String &&
        oldRev != nullptr &&
        obj->setString("_id", id) &&
        obj->setString("_rev", rev.data())) {
        doc = document_ptr(new Document(obj));
    } else {
        rs::scriptobject::utils::ObjectVector tempDefn = {
            std::make_pair("_id", rs::scriptobject::utils::VectorValue(id)),
            std::make_pair("_rev", rs::scriptobject::utils::VectorValue(rev.data()))
        };
        
        rs::scriptobject::utils::ScriptObjectVectorSource tempSource(tempDefn);
        
        auto tempObj = rs::scriptobject::ScriptObjectFactory::CreateObject(tempSource);
        
        auto newObj = rs::scriptobject::ScriptObject::Merge(obj, tempObj, rs::scriptobject::ScriptObject::MergeStrategy::Front);
        
        doc = document_ptr(new Document(newObj));
    }
    
    return doc;
}

const char* Document::getId() {
    return id_;
}

const char* Document::getRev() {
    return rev_;
}

bool Document::ValidateHashField(const char* name) {
    return name != nullptr && std::strcmp(name, "_id") != 0 && std::strcmp(name, "_rev") != 0;
}

void Document::FormatRevision(long version, const rs::scriptobject::ScriptObjectHash& digest, RevString& rev) {
    std::snprintf(rev.data(), rev.size(), "%lu-%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
            version, digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7],
            digest[8], digest[9], digest[10], digest[11], digest[12], digest[13], digest[14], digest[15]);
}