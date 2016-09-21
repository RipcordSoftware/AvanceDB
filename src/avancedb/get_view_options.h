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

#ifndef RS_AVANCEDB_GET_VIEW_OPTIONS_H
#define RS_AVANCEDB_GET_VIEW_OPTIONS_H

#include "get_all_documents_options.h"
#include "map_reduce_query_key.h"

class GetViewOptions final : public GetAllDocumentsOptions  {
public:
    GetViewOptions(const rs::httpserver::QueryString& qs);
    
    bool Reduce() const;
    bool Group() const;    
    uint64_t GroupLevel() const;
    
    map_reduce_query_key_ptr StartKeyObj() const;
    map_reduce_query_key_ptr EndKeyObj() const;
    
private:

    mutable boost::optional<bool> reduce_;
    mutable boost::optional<bool> group_;
    mutable boost::optional<uint64_t> groupLevel_;
};

#endif /* RS_AVANCEDB_GET_VIEW_OPTIONS_H */

