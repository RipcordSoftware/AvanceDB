#include "databases.h"

#include <algorithm>

bool Databases::AddDatabase(const char* name) {
    if (!IsDatabase(name)) {
        databases_.emplace(name, true);
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

std::vector<std::string> Databases::GetDatabases() {
    std::vector<std::string> databases;
    databases.reserve(databases_.size());
    
    std::for_each(databases_.cbegin(), databases_.cend(), [&](const std::pair<std::string, bool>& item) {
        databases.push_back(item.first);
    });
    
    return std::move(databases);
}