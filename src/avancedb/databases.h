#ifndef DATABASES_H
#define	DATABASES_H

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