#include "config.h"

#include <algorithm>

#include <boost/thread.hpp>

unsigned Config::GetCPUCount() {
    auto cores = std::max(2u, boost::thread::hardware_concurrency());
    return cores;
}