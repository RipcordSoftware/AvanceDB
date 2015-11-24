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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <map>
#include <string>
#include <cstring>

#include "types.h"
#include "document_attachment.h"

class Document final : public boost::enable_shared_from_this<Document>, private boost::noncopyable {
public:
    using attachment_collection = std::map<std::string, document_attachment_ptr>;

    class Less {
    public:
        bool operator()(const document_ptr& a, const document_ptr& b) {
            return std::strcmp(a->id_, b->id_) < 0;
        }
    };
    
    class Equal {
    public:
        bool operator()(const document_ptr& a, const document_ptr& b) {
            return std::strcmp(a->id_, b->id_) == 0;
        }
    };
    
    class Compare {
    public:      
        Compare(const char* id) : id_(id) {}
        
        int operator()(const document_ptr& doc) {
            return std::strcmp(id_, doc->id_);
        }
        
    private:
        const char* id_;
    };
    
    static document_ptr Create(const char* id, script_object_ptr obj, sequence_type seqNum, bool incrementRev = true);
    
    const char* getId() const;
    std::uint64_t getIdHash() const;
    static std::uint64_t getIdHash(const char*);
    const char* getRev() const;
    sequence_type getUpdateSequence() const;
    
    const script_object_ptr getObject() const;   
        
private:
    
    friend document_ptr boost::make_shared<document_ptr::element_type>(script_object_ptr&, sequence_type&);

    Document(script_object_ptr obj, sequence_type seqNum);
    
    static bool ValidateHashField(const char*);
    
    script_object_ptr obj_;
    const char* id_;
    const char* rev_;
    const sequence_type seqNum_;

    attachment_collection attachments_;
};

#endif	/* DOCUMENT_H */

