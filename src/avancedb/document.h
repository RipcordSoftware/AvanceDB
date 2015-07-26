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
    
    static document_ptr Create(const char* id, script_object_ptr obj);
    
    const char* getId();
    const char* getRev();
    
private:        
    using RevString = std::array<char, 20 + 1 + 32 + 1>;
    
    Document(script_object_ptr obj);
    
    static bool ValidateHashField(const char*);
    static void FormatRevision(long version, const rs::scriptobject::ScriptObjectHash& digest, RevString& rev);
    
    script_object_ptr obj_;
    const char* id_;
    const char* rev_;

};

#endif	/* DOCUMENT_H */

