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

#include "map_reduce_thread_pool.h"

#include "config.h"
#include "set_thread_name.h"

MapReduceThreadPool::MapReduceThreadPool() : 
        defaultRuntime_(Config::SpiderMonkey::GetHeapSize(), Config::SpiderMonkey::GetEnableBaselineCompiler(), Config::SpiderMonkey::GetEnableIonCompiler()) {
    
}

void MapReduceThreadPool::Start() {
    boost::call_once(initFlag_, [&]() {
        ThreadPoolOptions threadPoolOptions;
        threadPoolOptions.threads_count = Config::GetCPUCount() * Config::MapReduce::GetCPUMultiplier();

        threadPoolRuntimes_.resize(threadPoolOptions.threads_count);

        threadPoolOptions.onStart = [&](size_t id){
            SetThreadName::Set("MapReduceWorker");

            auto rt = new rs::jsapi::Runtime(Config::SpiderMonkey::GetHeapSize(), Config::SpiderMonkey::GetEnableBaselineCompiler(), Config::SpiderMonkey::GetEnableIonCompiler());
            threadPoolRuntimes_[id].reset(rt);
        };

        threadPoolOptions.onStop = [&](size_t id) {
            threadPoolRuntimes_[id].release();
        };

        threadPool_.reset(new ThreadPool(threadPoolOptions));
    });
}

rs::jsapi::Runtime& MapReduceThreadPool::GetThreadRuntime() {
    auto id = Worker::getWorkerIdForCurrentThread();
    return *(threadPoolRuntimes_[id]);
}