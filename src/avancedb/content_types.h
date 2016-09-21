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

#ifndef CONTENT_TYPES_H
#define CONTENT_TYPES_H

struct ContentTypes final {
    static constexpr const char* textPlain{"text/plain"};
    static constexpr const char* applicationJson{"application/json"};
    static constexpr const char* applicationMsgPack{"application/msgpack"};
    static constexpr const char* applicationOctetStream{"application/octet-stream"};
    
    struct Utf8 final {
        static constexpr const char* textPlain{"text/plain; charset=utf-8"};
        static constexpr const char* applicationJson{"application/json; charset=utf-8"};
    };
};

#endif	/* CONTENT_TYPE_H */

