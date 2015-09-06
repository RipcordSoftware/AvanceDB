#ifndef UUID_HELPER_H
#define UUID_HELPER_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

class UuidHelper final {
public:
    using UuidString = char[33];
    using UuidGenerator = boost::uuids::random_generator;
    
    UuidHelper() = delete;
    UuidHelper(const UuidHelper& orig) = delete;
    
    static bool FormatUuid(const boost::uuids::uuid&, UuidString buffer);
    
private:

};

#endif	/* UUID_HELPER_H */

