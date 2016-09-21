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

#include "uuid_helper.h"

bool UuidHelper::FormatUuid(const boost::uuids::uuid& uuid, UuidString& buffer) {
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