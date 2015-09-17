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

#include "databases.h"

#include <algorithm>

#include "database.h"

bool Databases::AddDatabase(const char* name) {
    std::lock_guard<std::mutex> lock(databasesMutex_);
    bool added = false;

    if (databases_.find(name) == databases_.cend()) {
        databases_.emplace(name, Database::Create(name));
        added = true;
    }
    
    return added;
}

bool Databases::RemoveDatabase(const char* name) {
    std::lock_guard<std::mutex> lock(databasesMutex_);
    return databases_.erase(name) == 1;
}

bool Databases::IsDatabase(const char* name) {
    std::lock_guard<std::mutex> lock(databasesMutex_);
    return databases_.find(name) != databases_.cend();
}

database_ptr Databases::GetDatabase(const char* name) {
    std::lock_guard<std::mutex> lock(databasesMutex_);
    
    auto iter = databases_.find(name);
    return iter != databases_.cend() ? iter->second : nullptr;
}

std::vector<std::string> Databases::GetDatabases() {
    std::vector<std::string> databases;
    databases.reserve(databases_.size());
    
    if (true) {
        std::lock_guard<std::mutex> lock(databasesMutex_);
        std::for_each(databases_.cbegin(), databases_.cend(), [&](const std::pair<std::string, database_ptr>& item) {
            databases.push_back(item.first);
        });
    }
    
    return std::move(databases);
}