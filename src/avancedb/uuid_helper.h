/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2016 Ripcord Software
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

#ifndef UUID_HELPER_H
#define UUID_HELPER_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include <array>

class UuidHelper final {
public:
    using UuidString = std::array<char, 33>;
    using UuidGenerator = boost::uuids::random_generator;
    
    UuidHelper() = delete;
    UuidHelper(const UuidHelper& orig) = delete;
    
    static bool FormatUuid(const boost::uuids::uuid&, UuidString& buffer);
    
private:

};

#endif	/* UUID_HELPER_H */
