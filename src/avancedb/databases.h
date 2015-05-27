#ifndef DATABASES_H
#define	DATABASES_H

#include <map>
#include <vector>
#include <string>

class Databases {
public:
    
    bool AddDatabase(const char*);
    bool RemoveDatabase(const char*);
    bool IsDatabase(const char*);
    std::vector<std::string> GetDatabases();
    
private:
    std::map<std::string, bool> databases_;
};

#endif	/* DATABASES_H */