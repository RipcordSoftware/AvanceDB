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

#ifndef BULK_DOCUMENTS_RESULT_H
#define BULK_DOCUMENTS_RESULT_H

#include <vector>

struct BulkDocumentsResult final {
    BulkDocumentsResult(const char* id, const char* rev) : 
        ok_(true), id_(id), rev_(rev) {}

    BulkDocumentsResult(const char* id, const char* error, const char* reason) : 
        ok_(false), id_(id), error_(error), reason_(reason) {}

    const bool ok_;
    const std::string id_;
    const std::string rev_;        
    const std::string error_;
    const std::string reason_;
};

using BulkDocumentsResults = std::vector<BulkDocumentsResult>;

#endif	/* BULK_DOCUMENTS_RESULT_H */
