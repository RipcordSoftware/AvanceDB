#ifndef HTTP_SERVER_LOG_H
#define	HTTP_SERVER_LOG_H

#include "libhttpserver.h"

#include <ctime>

class HttpServerLog final {            
public:

    HttpServerLog() = delete;
    HttpServerLog(const HttpServerLog&) = delete;
    HttpServerLog& operator=(const HttpServerLog&) = delete;    
    
    static void Append(rs::httpserver::socket_ptr socket, rs::httpserver::request_ptr request, rs::httpserver::response_ptr response, const std::time_t& start, long duration);    
    
private:
    
    static void GetTimestamp(const std::time_t& time, int& year, int& month, int& day, int& hour, int& min, int& sec);
    static unsigned GetMonth(long elapsedSeconds, bool isLeap);
    static unsigned GetDay(long elapsedSeconds, unsigned month, bool isLeap);    

};

#endif	/* HTTP_SERVER_LOG_H */

