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

#include "map_reduce_thread_pool.h"

#include "config.h"
#include "set_thread_name.h"

MapReduceThreadPool::map_reduce_thread_pool_ptr mapReduceThreadPool_;

MapReduceThreadPool::MapReduceThreadPool(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler) :
        defaultRuntime_(jsapiHeapSize, enableBaselineCompiler, enableIonCompiler) {
    
}

MapReduceThreadPool::map_reduce_thread_pool_ptr MapReduceThreadPool::Start(std::uint32_t jsapiHeapSize, bool enableBaselineCompiler, bool enableIonCompiler) {
    auto threadPool = boost::make_shared<map_reduce_thread_pool_ptr::element_type>(jsapiHeapSize, enableBaselineCompiler, enableIonCompiler);

    ThreadPoolOptions threadPoolOptions;
    threadPoolOptions.threads_count = Config::GetCPUCount() * Config::MapReduce::GetCPUMultiplier();

    threadPool->threadPoolRuntimes_.resize(threadPoolOptions.threads_count);

    threadPoolOptions.onStart = [=](size_t id){
        SetThreadName::Set("MapReduceWorker");

        auto rt = new rs::jsapi::Runtime(jsapiHeapSize, enableBaselineCompiler, enableIonCompiler);
        threadPool->threadPoolRuntimes_[id].reset(rt);
    };

    threadPoolOptions.onStop = [=](size_t id) {
        threadPool->threadPoolRuntimes_[id].release();
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

rs::jsapi::Runtime& MapReduceThreadPool::GetThreadRuntime() {
    auto id = Worker::getWorkerIdForCurrentThread();
    return *(threadPoolRuntimes_[id]);
}