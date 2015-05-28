#include "databases.h"

#include <algorithm>

bool Databases::AddDatabase(const char* name) {
    if (!IsDatabase(name)) {
        databases_.emplace(name, Database::Create(name));
        return true;
    } else {
        return false;
    }
}

bool Databases::RemoveDatabase(const char* name) {
    return databases_.erase(name) == 1;
}

bool Databases::IsDatabase(const char* name) {
    return databases_.find(name) != databases_.cend();
}

Database::database_ptr Databases::GetDatabase(const char* name) {
    auto iter = databases_.find(name);
    return iter != databases_.cend() ? iter->second : nullptr;
}

std::vector<std::string> Databases::GetDatabases() {
    std::vector<std::string> databases;
    databases.reserve(databases_.size());
    
    std::for_each(databases_.cbegin(), databases_.cend(), [&](const std::pair<std::string, Database::database_ptr>& item) {
        databases.push_back(item.first);
    });
    
    return std::move(databases);
}