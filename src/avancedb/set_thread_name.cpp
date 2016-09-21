/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015 Ripcord Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "set_thread_name.h"

#if __GNUG__ && not defined(__clang__)
#include <pthread.h>

bool SetThreadName::Set(std::thread& thread, const char* name) {
    auto status = ::pthread_setname_np(thread.native_handle(), name) == 0;
    return status;
}

bool SetThreadName::Set(const char* name) {
    auto status = pthread_setname_np(::pthread_self(), name) == 0;
    return status;
}
#else
bool SetThreadName::Set(std::thread&, const char*) {
    return false;
}

bool SetThreadName::Set(const char*) {
    return false;
}
#endif