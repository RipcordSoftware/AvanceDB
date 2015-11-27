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

#ifndef DOCUMENT_ATTACHMENT_H
#define DOCUMENT_ATTACHMENT_H

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "types.h"

class DocumentAttachment final : public boost::enable_shared_from_this<DocumentAttachment>, private boost::noncopyable {
public:
    using value_type = unsigned char;
    using size_type = std::size_t;        
    
    static document_attachment_ptr Create(const char* name, const char* contentType, const char* digest, std::vector<value_type>&& data, size_type size = 0);
    
    const std::string& Name() const;
    const std::string& ContentType() const;
    size_type Size() const;
    const value_type* Data() const;
    const std::string& Digest() const;
    
private:
    friend document_attachment_ptr boost::make_shared<document_attachment_ptr::element_type>(const char*&, const char*&, const char*&, std::vector<value_type>&&, size_type&);
    
    DocumentAttachment(const char* name, const char* contentType, const char* digest, std::vector<value_type>&& data, size_type size);

    static const char* ParseDigest(const std::string&);
    
    const std::string name_;
    const std::string contentType_;
    const std::vector<value_type> data_;
    const size_type size_;
    const std::string digest_;
};

#endif	/* DOCUMENT_ATTACHMENT_H */
