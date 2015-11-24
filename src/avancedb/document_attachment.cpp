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

#include "document_attachment.h"

#include <utility>

DocumentAttachment::DocumentAttachment(const char* name, const char* contentType, std::vector<value_type>&& data, const char* digest) :
    name_(name), contentType_(contentType), data_(std::move(data)), digest_(digest) {

}

document_attachment_ptr DocumentAttachment::Create(const char* name, const char* contentType, std::vector<value_type>&& data, const char* digest) {
    return boost::make_shared<document_attachment_ptr::element_type>(name, contentType, std::forward<decltype(data)>(data), digest);
}

const std::string& DocumentAttachment::Name() const {
    return name_;
}

const std::string& DocumentAttachment::ContentType() const {
    return contentType_;
}

DocumentAttachment::size_type DocumentAttachment::Size() const {
    return data_.size();
}

const DocumentAttachment::value_type* DocumentAttachment::Data() const {
    return data_.data();
}

const std::string& DocumentAttachment::Digest() const {
    return digest_;
}