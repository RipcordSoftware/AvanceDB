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

#include "map_reduce_thread_pool.h"

#include "config.h"
#include "set_thread_name.h"

MapReduceThreadPool::map_reduce_thread_pool_ptr mapReduceThreadPool_;

MapReduceThreadPool::map_reduce_thread_pool_ptr MapReduceThreadPool::Start(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler) {
    auto threadPool = boost::make_shared<map_reduce_thread_pool_ptr::element_type>();

    ThreadPoolOptions threadPoolOptions;
    threadPoolOptions.threads_count = Config::Environment::CpuCount() * Config::MapReduce::GetCPUMultiplier();

    threadPool->threadPoolContexts_.resize(threadPoolOptions.threads_count);

    threadPoolOptions.onStart = [=](size_t id){
        SetThreadName::Set("avancedb-mapred");

        auto rt = new rs::jsapi::Context(jsapiHeapSize, JS::DefaultNurseryBytes, enableBaselineCompiler, enableIonCompiler);
        threadPool->threadPoolContexts_[id].reset(rt);
    };

    threadPoolOptions.onStop = [=](size_t id) {
        threadPool->threadPoolContexts_[id].release();
    };
    
    threadPool->threadPool_.reset(new ThreadPool(threadPoolOptions));
    
    mapReduceThreadPool_ = threadPool;

    return mapReduceThreadPool_;
}

MapReduceThreadPool::map_reduce_thread_pool_ptr MapReduceThreadPool::Get() {
    return mapReduceThreadPool_;
}

void MapReduceThreadPool::Stop() {
    mapReduceThreadPool_.reset();
}

rs::jsapi::Context& MapReduceThreadPool::GetThreadContext(size_t threadId) {
    return *(threadPoolContexts_[threadId]);
}