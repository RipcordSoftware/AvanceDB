/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2017 Ripcord Software
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

#include "rest_config.h"

#define REST_CONFIG_QUERY_SERVERS R"({"javascript":"libjsapi"})"
#define REST_CONFIG_NATIVE_QUERY_SERVERS R"({})"

const char* RestConfig::getConfig() {
    return 
        R"({)"
        R"("query_servers":)" REST_CONFIG_QUERY_SERVERS
        R"(})";
}

const char* RestConfig::getQueryServers() {
    return REST_CONFIG_QUERY_SERVERS;
}

const char* RestConfig::getNativeQueryServers() {
    return REST_CONFIG_NATIVE_QUERY_SERVERS;
}