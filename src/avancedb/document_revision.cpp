#include "document_revision.h"

#include <algorithm>
#include <cstring>

#include "rest_exceptions.h"

DocumentRevision::DocumentRevision(uint64_t version, const Digest& digest) : version_(version) {
    std::copy_n(digest, sizeof(digest_), digest_);
}

bool DocumentRevision::Validate(const char* revString, bool throwOnFail) {
    auto valid = false;
    
    if (revString != nullptr) {
        auto length = std::strlen(revString);
        
        if (length <= maxRevStringLength_) {
            unsigned index = 0;
            while (revString[index] != '\0' && std::isdigit(revString[index])) {
                ++index;
            }    
            
            if (index > 0 && revString[index] == '-') {            
                ++index;

                auto digestStart = index;

                while (revString[index] != '\0' && std::isxdigit(revString[index])) {
                    ++index;
                }

                if (revString[index] == '\0' && (index - digestStart) == digestStringLength_) {
                    valid = true;
                }
            }
        }
    }
    
    if (!valid && throwOnFail) {
        throw InvalidRevisionFormat{};
    }
    
    return valid;
}

DocumentRevision DocumentRevision::Parse(const char* revString) {
    unsigned index = 0;
    while (revString[index] != '\0' && std::isdigit(revString[index])) {
        ++index;
    }    
    
    if (index == 0 || revString[index] != '-') {
        throw InvalidRevisionFormat{};
    }
    
    ++index;
    
    auto digestStart = index;
    
    while (revString[index] != '\0' && std::isxdigit(revString[index])) {
        ++index;
    }
    
    auto digestEnd = index;
    
    if (revString[digestEnd] != '\0' || (digestEnd - digestStart) != digestStringLength_) {
        throw InvalidRevisionFormat{};
    }        
    
    auto version = std::strtoul(revString, nullptr, 10);
    
    Digest digest;
    for (auto i = digestStart, index = 0u; i < digestEnd; i += 2, index++) {
        digest[index] = GetCharNumericValue(revString[i]) << 4;  
        digest[index] |= GetCharNumericValue(revString[i + 1]);
    }
    
    return DocumentRevision(version, digest);
}

DocumentRevision& DocumentRevision::Increment() {
    ++version_;    
    return *this;
}

DocumentRevision&  DocumentRevision::FormatRevision(RevString& rev) {
    FormatRevision(version_, digest_, rev);
    return *this;
}

void DocumentRevision::FormatRevision(version_type version, const Digest& digest, RevString& rev) {
    std::snprintf(rev.data(), rev.size(), "%lu-%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
            version, digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7],
            digest[8], digest[9], digest[10], digest[11], digest[12], digest[13], digest[14], digest[15]);
}

unsigned char DocumentRevision::GetCharNumericValue(char ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else {
        return (ch | 0x20) - 0x61 + 10;
    }
}