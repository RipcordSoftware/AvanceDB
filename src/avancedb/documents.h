#ifndef DOCUMENTS_H
#define	DOCUMENTS_H

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>

#include "types.h"

class Database;

class Documents final : public boost::enable_shared_from_this<Documents>, private boost::noncopyable {
public:

    static documents_ptr Create(database_ptr db);
    
private:
    
    Documents(database_ptr db);
    
    database_ptr db_;

};

#endif	/* DOCUMENTS_H */
