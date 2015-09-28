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

#ifndef TYPES_H
#define TYPES_H

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
using document_array_ptr = boost::shared_ptr<std::vector<document_ptr>>;

class MapReduceResult;
using map_reduce_result_ptr = boost::shared_ptr<MapReduceResult>;
using map_reduce_result_array_ptr = boost::shared_ptr<std::vector<map_reduce_result_ptr>>;

using script_object_ptr = rs::scriptobject::ScriptObjectPtr;
using script_array_ptr = rs::scriptobject::ScriptArrayPtr;

using sequence_type = unsigned long;

#endif	/* TYPES_H */

