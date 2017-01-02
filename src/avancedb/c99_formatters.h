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

#ifndef RS_AVANCEDB_SCRIPT_C99_FORMATTERS_H
#define RS_AVANCEDB_SCRIPT_C99_FORMATTERS_H

#include <inttypes.h>

#ifndef PRId64
#ifdef _LP64
#define PRId64 "lld"
#else
#define PRId64 "ld"
#endif /* _LP64 */
#endif /* PRId64 */

#ifndef PRIu64
#ifdef _LP64
#define PRIu64 "llu"
#else
#define PRIu64 "lu"
#endif /* _LP64 */
#endif /* PRIu64 */

#endif /* RS_AVANCEDB_SCRIPT_C99_FORMATTERS_H */

