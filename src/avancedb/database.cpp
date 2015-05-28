#include "database.h"

#include <boost/chrono.hpp>

Database::Database(const char* name) : name_(name), instanceStartTime_(Now()),
    committedUpdateSeq_(0), updateSeq_(0), purgeSeq_(0), dataSize_(0), diskSize_(0),
    docCount_(0), docDelCount_(0) {

}

Database::database_ptr Database::Create(const char* name) {
    return database_ptr(new Database(name));
}

unsigned long Database::Now() {
    auto now = boost::chrono::system_clock::now().time_since_epoch();
    return boost::chrono::duration_cast<boost::chrono::microseconds>(now).count();
}