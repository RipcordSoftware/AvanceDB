#ifndef SCRIPT_OBJECT_STREAM_H
#define	SCRIPT_OBJECT_STREAM_H

#include <sstream>

#include "types.h"

class ScriptObjectStream final {
public:
    ScriptObjectStream() = delete;    
    ScriptObjectStream(const ScriptObjectStream&) = delete;
    
    static std::string Serialize(script_object_ptr obj);
    
private:
    
    static void Serialize(std::stringstream& stream, script_object_ptr obj);
    static void Serialize(std::stringstream& stream, script_array_ptr arr);
    
    static void Append(std::stringstream& stream, const char* name, const char* value);
    static void Append(std::stringstream& stream, const char* name, int value);
    static void Append(std::stringstream& stream, const char* name, unsigned value);
    static void Append(std::stringstream& stream, const char* name, uint64_t value);
    static void Append(std::stringstream& stream, const char* name, int64_t value);
    static void Append(std::stringstream& stream, const char* name, double value);
    static void Append(std::stringstream& stream, const char* name, bool value);
    static void AppendNull(std::stringstream& stream, const char* name);
    static void Append(std::stringstream& stream, const char* name, script_object_ptr obj);
    static void Append(std::stringstream& stream, const char* name, script_array_ptr arr);
    
    static void Append(std::stringstream& stream, const char* value);
    static void Append(std::stringstream& stream, int value);
    static void Append(std::stringstream& stream, unsigned value);
    static void Append(std::stringstream& stream, uint64_t value);
    static void Append(std::stringstream& stream, int64_t value);
    static void Append(std::stringstream& stream, double value);
    static void Append(std::stringstream& stream, bool value);
    static void AppendNull(std::stringstream& stream);

};

#endif	/* SCRIPT_OBJECT_STREAM_H */

