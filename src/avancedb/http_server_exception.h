#ifndef HTTP_SERVER_EXCEPTION_H
#define	HTTP_SERVER_EXCEPTION_H

#include <exception>
#include <string>

class HttpServerException : public std::exception {
public:
    
    HttpServerException(unsigned statusCode, const char* description, const char* body = nullptr, const char* contentType = nullptr) : 
        statusCode_(statusCode), description_(description), body_(body), contentType_(contentType) {        
    }
    
    virtual const char* what() const noexcept { return Description(); }
    
    unsigned StatusCode() const noexcept { return statusCode_; }
    const char* Description() const noexcept { return description_.c_str(); }
    const char* Body() const noexcept { return body_.c_str(); }
    const char* ContentType() const noexcept { return contentType_.c_str(); }
    
private:
    const unsigned statusCode_;
    const std::string description_;
    const std::string body_;
    const std::string contentType_;
        
};

#endif	/* HTTP_SERVER_EXCEPTION_H */

