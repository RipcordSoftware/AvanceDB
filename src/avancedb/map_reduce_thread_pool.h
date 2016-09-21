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

#ifndef RS_AVANCEDB_MAP_REDUCE_THREAD_POOL_H
#define RS_AVANCEDB_MAP_REDUCE_THREAD_POOL_H

#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "libjsapi.h"

#include "thread_pool.hpp"

class MapReduceThreadPool final : public boost::enable_shared_from_this<MapReduceThreadPool> {    
public:
    using map_reduce_thread_pool_ptr = boost::shared_ptr<MapReduceThreadPool>;
    
    MapReduceThreadPool(const MapReduceThreadPool&) = delete;
    MapReduceThreadPool& operator=(const MapReduceThreadPool&) = delete;    
    
    static map_reduce_thread_pool_ptr Start(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler);
    static map_reduce_thread_pool_ptr Get();

    static void Stop();
    
    template <typename Handler>
    void Post(Handler&& handler) {
        threadPool_->post(handler);
    }   
    
    rs::jsapi::Runtime& GetThreadRuntime();
    
private:
    friend map_reduce_thread_pool_ptr boost::make_shared<map_reduce_thread_pool_ptr::element_type>(std::uint32_t&, bool&, bool&);
    
    MapReduceThreadPool(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler);

    rs::jsapi::Runtime defaultRuntime_;
    std::vector<std::unique_ptr<rs::jsapi::Runtime>> threadPoolRuntimes_;
    std::unique_ptr<ThreadPool> threadPool_;
};

class MapReduceThreadPoolScope final {
public:
    MapReduceThreadPoolScope(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler) {
        MapReduceThreadPool::Start(jsapiHeapSize, enableBaselineCompiler, enableIonCompiler);
        started = true;
    }
    
    ~MapReduceThreadPoolScope() {
        if (started) {
            MapReduceThreadPool::Stop();
        }
    }
    
private:
    bool started{false};
};

#endif /* RS_AVANCEDB_MAP_REDUCE_THREAD_POOL_H */