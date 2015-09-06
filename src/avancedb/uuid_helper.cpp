#include "uuid_helper.h"

bool UuidHelper::FormatUuid(const boost::uuids::uuid& uuid, UuidString buffer) {
    auto status = false;
    
    auto size = uuid.size();
    if ((size * 2) < sizeof(UuidString)) {            
        auto uuidPtr = uuid.data;
        decltype(size) i = 0, j = 0;
        for (; i < size; ++i, ++uuidPtr, j += 2) {
            buffer[j] = ((*uuidPtr) >> 4) + 0x30;
            buffer[j] += buffer[j] > 0x39 ? 0x27 : 0;

            buffer[j + 1] = ((*uuidPtr) & 0x0f) + 0x30;
            buffer[j + 1] += buffer[j + 1] > 0x39 ? 0x27 : 0;               
        }

        buffer[j] = '\0';
        status = true;
    }
    
    return status;
}