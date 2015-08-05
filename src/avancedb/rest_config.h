#ifndef REST_CONFIG_H
#define	REST_CONFIG_H

class RestConfig final {
public:
    RestConfig() = delete;
    
    static const char* getConfig();
    
    static const char* getQueryServers();
    static const char* getNativeQueryServers();
    
private:

};

#endif	/* REST_CONFIG_H */

