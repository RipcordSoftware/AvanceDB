#ifndef TYPES_H
#define	TYPES_H

#include <boost/shared_ptr.hpp>

class Database;
using database_ptr = boost::shared_ptr<Database>;

class Documents;
using documents_ptr = boost::shared_ptr<Documents>;

#endif	/* TYPES_H */

