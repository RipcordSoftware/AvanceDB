#include "rest_config.h"

#define REST_CONFIG_QUERY_SERVERS R"({"javascript":"libjsapi"})"
#define REST_CONFIG_NATIVE_QUERY_SERVERS R"({})"

const char* RestConfig::getConfig() {
    return 
        R"({)"
        R"("query_servers":)" REST_CONFIG_QUERY_SERVERS
        R"(})";
}

const char* RestConfig::getQueryServers() {
    return REST_CONFIG_QUERY_SERVERS;
}

const char* RestConfig::getNativeQueryServers() {
    return REST_CONFIG_NATIVE_QUERY_SERVERS;
}