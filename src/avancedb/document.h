#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <cstring>

#include "types.h"

class Document final : public boost::enable_shared_from_this<Document>, private boost::noncopyable {
public:

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
    
    static document_ptr Create(const char* id, script_object_ptr obj, sequence_type seqNum);
    
    const char* getId() const;
    const char* getRev() const;
    sequence_type getUpdateSequence() const;
    
    const script_object_ptr getObject() const;
        
private:        

    Document(script_object_ptr obj, sequence_type seqNum);
    
    static bool ValidateHashField(const char*);
    
    script_object_ptr obj_;
    const char* id_;
    const char* rev_;
    const sequence_type seqNum_;

};

#endif	/* DOCUMENT_H */

