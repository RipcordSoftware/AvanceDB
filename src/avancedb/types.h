#ifndef TYPES_H
#define	TYPES_H

#include <boost/shared_ptr.hpp>

#include <vector>

#include "libscriptobject.h"

class Database;
using database_ptr = boost::shared_ptr<Database>;
using database_wptr = boost::weak_ptr<Database>;

class Documents;
using documents_ptr = boost::shared_ptr<Documents>;

class Document;
using document_ptr = boost::shared_ptr<Document>;
using document_array = std::vector<document_ptr>;

using script_object_ptr = rs::scriptobject::ScriptObjectPtr;
using script_array_ptr = rs::scriptobject::ScriptArrayPtr;

using sequence_type = unsigned long;

#endif	/* TYPES_H */

