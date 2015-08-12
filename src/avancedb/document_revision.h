#ifndef DOCUMENT_REVISION_H
#define	DOCUMENT_REVISION_H

#include <array>

class DocumentRevision {
public:    
    static const unsigned digestLength_ = 16;
    static const unsigned digestStringLength_ = digestLength_ * 2;
    static const unsigned maxRevStringLength_ = 20 + 1 + digestStringLength_;
    
    using version_type = uint64_t;
    using RevString = std::array<char, maxRevStringLength_ + 1>;
    using Digest = unsigned char[digestLength_];    
    
    DocumentRevision& Increment();
    
    static DocumentRevision Parse(const char*);
    static bool Validate(const char*, bool throwOnFail = false);
    
    DocumentRevision& FormatRevision(RevString& rev);
    static void FormatRevision(version_type version, const Digest& digest, RevString& rev);
    
private:    
        
    DocumentRevision(uint64_t version, const Digest& digest);
    
    static unsigned char GetCharNumericValue(char);
    
    version_type version_;
    Digest digest_;
};

#endif	/* DOCUMENT_REVISION_H */
