#ifndef TYPES_H
#define	TYPES_H

#include <boost/shared_ptr.hpp>

#include "libscriptobject.h"

class Database;
using database_ptr = boost::shared_ptr<Database>;
using database_wptr = boost::weak_ptr<Database>;

class Documents;
using documents_ptr = boost::shared_ptr<Documents>;

class Document;
using document_ptr = boost::shared_ptr<Document>;

using script_object_ptr = rs::scriptobject::ScriptObjectPtr;

#endif	/* TYPES_H */

