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

#ifndef MAP_REDUCE_THREAD_POOL_H
#define MAP_REDUCE_THREAD_POOL_H

#include <boost/thread.hpp>

#include "libjsapi.h"

#include "thread_pool.hpp"

class MapReduceThreadPool final {    
public:  
    MapReduceThreadPool(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler);
    MapReduceThreadPool(const MapReduceThreadPool&) = delete;
    MapReduceThreadPool& operator=(const MapReduceThreadPool&) = delete;    
    
    void Start();
    
    template <typename Handler>
    void Post(Handler&& handler) {
        threadPool_->post(handler);
    }   
    
    rs::jsapi::Runtime& GetThreadRuntime();
    
private:
    const std::uint32_t jsapiHeapSize_;
    const bool enableBaselineCompiler_;
    const bool enableIonCompiler_;
    
    rs::jsapi::Runtime defaultRuntime_;

    std::vector<std::unique_ptr<rs::jsapi::Runtime>> threadPoolRuntimes_;
    std::unique_ptr<ThreadPool> threadPool_;
};

#endif	/* MAP_REDUCE_THREAD_POOL_H */

