#ifndef DATABASES_H
#define	DATABASES_H

#include <map>
#include <vector>
#include <string>

#include "database.h"

class Databases {
public:
    
    bool AddDatabase(const char*);
    bool RemoveDatabase(const char*);
    Database::database_ptr GetDatabase(const char*);
    bool IsDatabase(const char*);
    std::vector<std::string> GetDatabases();
    
private:
    std::map<std::string, Database::database_ptr> databases_;
};

#endif	/* DATABASES_H */