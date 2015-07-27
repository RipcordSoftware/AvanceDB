#include "script_object_stream.h"

#include "libscriptobject.h"

std::string ScriptObjectStream::Serialize(script_object_ptr obj) {
    std::stringstream stream;
    
    Serialize(stream, obj);
    
    return stream.str();
}

void ScriptObjectStream::Serialize(std::stringstream& stream, script_object_ptr obj) {
    stream << '{';
    
    auto count = obj->getCount();
    for (decltype(count) i = 0; i < count; ++i) {
        if (i > 0) {
            stream << ',';
        }
        
        auto name = obj->getName(i);
        auto type = obj->getType(i);
        switch (type) {
            case rs::scriptobject::ScriptObjectType::Array: Append(stream, name, obj->getArray(i)); break;
            case rs::scriptobject::ScriptObjectType::Boolean: Append(stream, name, obj->getBoolean(i)); break;
            case rs::scriptobject::ScriptObjectType::Double: Append(stream, name, obj->getDouble(i)); break;
            case rs::scriptobject::ScriptObjectType::Int32: Append(stream, name, obj->getInt32(i)); break;
            case rs::scriptobject::ScriptObjectType::Null: AppendNull(stream, name); break;
            case rs::scriptobject::ScriptObjectType::Object: Append(stream, name, obj->getObject(i)); break;
            case rs::scriptobject::ScriptObjectType::String: Append(stream, name, obj->getString(i)); break;
        }
    }
 
    stream << '}';
}

void ScriptObjectStream::Serialize(std::stringstream& stream, script_array_ptr arr) {
    stream << '[';
    
    auto count = arr->getCount();
    for (decltype(count) i = 0; i < count; ++i) {
        if (i > 0) {
            stream << ',';
        }
        
        auto type = arr->getType(i);
        switch (type) {
            case rs::scriptobject::ScriptObjectType::Array: Serialize(stream, arr->getArray(i)); break;
            case rs::scriptobject::ScriptObjectType::Boolean: Append(stream, arr->getBoolean(i)); break;
            case rs::scriptobject::ScriptObjectType::Double: Append(stream, arr->getDouble(i)); break;
            case rs::scriptobject::ScriptObjectType::Int32: Append(stream, arr->getInt32(i)); break;
            case rs::scriptobject::ScriptObjectType::Null: AppendNull(stream); break;
            case rs::scriptobject::ScriptObjectType::Object: Serialize(stream, arr->getObject(i)); break;
            case rs::scriptobject::ScriptObjectType::String: Append(stream, arr->getString(i)); break;
        }
    }
 
    stream << ']';
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, const char* value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, int value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, unsigned value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, int64_t value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, uint64_t value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, double value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, bool value) {
    stream << "\"" << name << "\":";
    Append(stream, value);
}

void ScriptObjectStream::AppendNull(std::stringstream& stream, const char* name) {
    stream << "\"" << name << "\":";
    AppendNull(stream);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, script_object_ptr obj) {
    stream << "\"" << name << "\":";
    Serialize(stream, obj);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* name, script_array_ptr arr) {
    stream << "\"" << name << "\":";
    Serialize(stream, arr);
}

void ScriptObjectStream::Append(std::stringstream& stream, const char* value) {
    stream << "\"" << value << "\"";
}

void ScriptObjectStream::Append(std::stringstream& stream, int value) {
    stream << value;
}

void ScriptObjectStream::Append(std::stringstream& stream, unsigned value) {
    stream << value;
}

void ScriptObjectStream::Append(std::stringstream& stream, int64_t value) {
    stream << value;
}

void ScriptObjectStream::Append(std::stringstream& stream, uint64_t value) {
    stream << value;
}

void ScriptObjectStream::Append(std::stringstream& stream, double value) {
    stream << value;
}

void ScriptObjectStream::Append(std::stringstream& stream, bool value) {
    stream << (value ? "true" : "false");
}

void ScriptObjectStream::AppendNull(std::stringstream& stream) {
    stream << "null";
}