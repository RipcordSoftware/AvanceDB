#include "databases.h"

#include <algorithm>

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

Database::database_ptr Databases::GetDatabase(const char* name) {
    std::lock_guard<std::mutex> lock(databasesMutex_);
    
    auto iter = databases_.find(name);
    return iter != databases_.cend() ? iter->second : nullptr;
}

std::vector<std::string> Databases::GetDatabases() {
    std::vector<std::string> databases;
    databases.reserve(databases_.size());
    
    if (true) {
        std::lock_guard<std::mutex> lock(databasesMutex_);
        std::for_each(databases_.cbegin(), databases_.cend(), [&](const std::pair<std::string, Database::database_ptr>& item) {
            databases.push_back(item.first);
        });
    }
    
    return std::move(databases);
}