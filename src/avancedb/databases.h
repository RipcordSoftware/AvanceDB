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

#ifndef DATABASES_H
#define DATABASES_H

#include <map>
#include <vector>
#include <string>
#include <mutex>

#include "types.h"

class Databases {
public:
    
    bool AddDatabase(const char*);
    bool RemoveDatabase(const char*);
    database_ptr GetDatabase(const char*);
    bool IsDatabase(const char*);
    std::vector<std::string> GetDatabases();
    
private:
    std::map<std::string, database_ptr> databases_;
    
    std::mutex databasesMutex_;
};

#endif	/* DATABASES_H */