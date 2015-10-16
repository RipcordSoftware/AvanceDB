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
#include "get_view_options.h"

GetViewOptions::GetViewOptions(const rs::httpserver::QueryString& qs) : GetAllDocumentsOptions(qs) {
    
}

bool GetViewOptions::Reduce() const {
    if (!reduce_.is_initialized()) {
        reduce_ = GetBoolean("reduce", false);
    }
    return reduce_.get();
}

bool GetViewOptions::Group() const {
    if (!group_.is_initialized()) {
        group_ = GetBoolean("group", false);
    }
    return group_.get();
}

uint64_t GetViewOptions::GroupLevel() const {
    if (!groupLevel_.is_initialized()) {
        groupLevel_ = GetUnsigned("group_level", 0);
    }
    return groupLevel_.get();
}

map_reduce_query_key_ptr GetViewOptions::StartKeyObj() const {
    map_reduce_query_key_ptr ptr{nullptr};
    
    if (HasStartKey()) {
        auto key = StartKey();
        auto id = StartKeyDocId();
        ptr = MapReduceQueryKey::Create(key.c_str(), id.c_str());
    }
    
    return ptr;
}

map_reduce_query_key_ptr GetViewOptions::EndKeyObj() const {
    map_reduce_query_key_ptr ptr{nullptr};
    
    if (HasEndKey()) {
        auto key = EndKey();
        auto id = EndKeyDocId();
        return MapReduceQueryKey::Create(key.c_str(), id.c_str());
    }
    
    return ptr;
}