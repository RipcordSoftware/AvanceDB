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

#include "document.h"

#include <cstring>
#include <cstdlib>

#include "script_object_vector_source.h"

#include "document_revision.h"
#include "city.h"
#include "base64_helper.h"

Document::Document(script_object_ptr obj, sequence_type seqNum) : obj_(obj), id_(obj->getString("_id")), rev_(obj->getString("_rev")), seqNum_(seqNum) {
}

document_ptr Document::Create(const char* id, script_object_ptr obj, sequence_type seqNum, bool incrementRev) {
    const char* oldRev = obj->getString("_rev", false);
    const char* newRev = nullptr;
    DocumentRevision::RevString newRevString;
    
    if (incrementRev || oldRev == nullptr) {
        // TODO: strtoul is unsafe
        auto oldVersion = oldRev != nullptr ? std::strtoul(oldRev, nullptr, 10) : 0;
        auto nextVersion = oldVersion + 1;
    
        rs::scriptobject::ScriptObjectHash digest;
        obj->CalculateHash(digest, &Document::ValidateHashField);

        DocumentRevision::FormatRevision(nextVersion, digest, newRevString);
        newRev = newRevString.data();
    }
    
    document_ptr doc;
    if (obj->getType("_id") == rs::scriptobject::ScriptObjectType::String &&
        oldRev != nullptr &&
        obj->setString("_id", id) &&
        (newRev == nullptr || obj->setString("_rev", newRev))) {
        doc = boost::make_shared<document_ptr::element_type>(obj, seqNum);
    } else {
        rs::scriptobject::utils::ObjectVector tempDefn = {
            std::make_pair("_id", rs::scriptobject::utils::VectorValue(id)),
            std::make_pair("_rev", rs::scriptobject::utils::VectorValue(newRev ? newRev : oldRev))
        };
        
        rs::scriptobject::utils::ScriptObjectVectorSource tempSource(tempDefn);
        
        auto tempObj = rs::scriptobject::ScriptObjectFactory::CreateObject(tempSource);
        
        auto newObj = rs::scriptobject::ScriptObject::Merge(obj, tempObj, rs::scriptobject::ScriptObject::MergeStrategy::Front);
        
        doc = boost::make_shared<document_ptr::element_type>(newObj, seqNum);
    }
    
    return doc;
}

const char* Document::getId() const {
    return id_;
}

std::uint64_t Document::getIdHash() const {
    return getIdHash(id_);
}

std::uint64_t Document::getIdHash(const char* id) {    
    return id != nullptr ? CityHash64(id, std::strlen(id)) : 0;
}

const char* Document::getRev() const {
    return rev_;
}

sequence_type Document::getUpdateSequence() const {
    return seqNum_;
}

const script_object_ptr Document::getObject() const {
    return obj_;
}

bool Document::ValidateHashField(const char* name) {
    return name != nullptr && std::strcmp(name, "_id") != 0 && std::strcmp(name, "_rev") != 0;
}

document_attachment_ptr Document::getAttachment(const char* name, bool includeBody) {    
    document_attachment_ptr attachment;
    
    auto attachmentsObj = obj_->getObject("_attachments", false);
    if (!!attachmentsObj) {        
        auto attachmentObj = attachmentsObj->getObject(name, false);
        if (!!attachmentObj) {
            auto contentType = attachmentObj->getString("content_type");
            auto digest = attachmentObj->getString("digest");

            if (includeBody) {
                auto encodedData = attachmentObj->getString("data");
                auto encodedDataSize = attachmentObj->getStringFieldLength("data");
                auto data = Base64Helper::Decode(encodedData, encodedDataSize > 0 ? encodedDataSize - 1 : 0);
                attachment = DocumentAttachment::Create(name, contentType, digest, std::move(data));
            } else {
                int lengthIndex = -1;
                auto size = attachmentObj->getType("length", lengthIndex) == rs::scriptobject::ScriptObjectType::UInt32 ? 
                    attachmentObj->getUInt32(lengthIndex) : attachmentObj->getUInt64(lengthIndex);

                attachment = DocumentAttachment::Create(name, contentType, digest, Base64Helper::buffer_type{}, size);
            }
        }
    }
    
    return attachment;
}

std::vector<document_attachment_ptr> Document::getAttachments() {
    std::vector<document_attachment_ptr> attachments;
    
    auto attachmentsObj = obj_->getObject("_attachments", false);
    if (!!attachmentsObj) {
        auto count = attachmentsObj->getCount();        
        attachments.reserve(count);
        
        for (decltype(count) i = 0; i < count; ++i) {
            auto type = attachmentsObj->getType(i);
            if (type == rs::scriptobject::ScriptObjectType::Object) {
                auto name = attachmentsObj->getName(i);
                auto attachmentObj = attachmentsObj->getObject(i);
                
                auto contentType = attachmentObj->getString("content_type");
                auto digest = attachmentObj->getString("digest");
                
                auto encodedData = attachmentObj->getString("data");
                auto encodedDataSize = attachmentObj->getStringFieldLength("data");
                auto data = Base64Helper::Decode(encodedData, encodedDataSize > 0 ? encodedDataSize - 1 : 0);
                
                auto attachment = DocumentAttachment::Create(name, contentType, digest, std::move(data));
                attachments.push_back(attachment);
            }
        }
    }
    
    return attachments;
}