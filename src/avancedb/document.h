#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "types.h"

class Document final : public boost::enable_shared_from_this<Document>, private boost::noncopyable {
public:
    
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

