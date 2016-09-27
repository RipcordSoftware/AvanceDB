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

#include "map_reduce.h"

#include <boost/make_shared.hpp>
#include <boost/scope_exit.hpp>
#include <boost/algorithm/string.hpp>

#include <memory>
#include <algorithm>
#include <condition_variable>
#include <functional>

#include "script_array_jsapi_key_value_source.h"
#include "map_reduce_result.h"
#include "map_reduce_shard_results.h"
#include "script_object_jsapi_source.h"
#include "script_array_jsapi_source.h"
#include "map_reduce_script_object_state.h"
#include "documents.h"
#include "config.h"
#include "set_thread_name.h"
#include "map_reduce_thread_pool.h"
#include "rest_exceptions.h"
#include "map_reduce_exception.h"

#include "script_object_factory.h"
#include "script_array_factory.h"

MapReduce::MapReduce() : mapReduceThreadPool_(MapReduceThreadPool::Get()) {
    
}

map_reduce_results_ptr MapReduce::Execute(const GetViewOptions& options, const MapReduceTask& task, document_collections_ptr_array colls) {
    auto language = task.Language();
    if (!boost::iequals("javascript", language)) {
        throw BadLanguageError{language};
    }
    
    std::mutex m;
    auto collsSize = colls.size();
    std::vector<map_reduce_shard_results_ptr> filteredResults;
    std::vector<rs::jsapi::ScriptException> scriptExceptions;
    std::atomic<int> threads(collsSize);
    std::condition_variable threadEnd;
    
    const auto skip = options.Skip();
    const auto limit = options.Limit();
    const auto startKey = options.StartKeyObj();
    const auto endKey = options.EndKeyObj();
    const auto inclusiveEnd = options.InclusiveEnd();
    const auto descending = options.Descending();
    
    // run the map
    for (auto& coll : colls) {
        mapReduceThreadPool_->Post([&](size_t threadId) {
            std::unique_lock<std::mutex> lock{m, std::defer_lock};
            
            try {
                BOOST_SCOPE_EXIT(&threads) { --threads; } BOOST_SCOPE_EXIT_END
                auto& rt = mapReduceThreadPool_->GetThreadRuntime(threadId);
                
                boost::unique_lock<document_collections_ptr_array::value_type::element_type> collLock{*coll};                
                document_array docs;
                coll->copy(docs, false);
                collLock.unlock();
                
                auto result = Execute(rt, task, docs);                                
                
                auto filteredResult = boost::make_shared<map_reduce_shard_results_ptr::element_type>(
                    result, skip + std::min(limit, result->size()), startKey, endKey, inclusiveEnd, descending);               

                lock.lock();
                filteredResults.emplace_back(filteredResult);
                threadEnd.notify_one();
            } catch (const rs::jsapi::ScriptException& ex) {
                lock.lock();
                threadEnd.notify_one();
                scriptExceptions.emplace_back(ex);
            }
        });
    }
    
    // wait for the map threads to finish
    std::unique_lock<std::mutex> lock{m};
    threadEnd.wait(lock, [&]() { return threads.load() == 0; });
    
    // we need to pass back any script exceptions to the caller
    if (scriptExceptions.size() > 0) {
        auto what = scriptExceptions[0].what();
        throw CompilationError{what};
    }
    
    // if the number of results doesn't match the number of colls then
    // we've most likely got a non-script exception during the map phase
    if (filteredResults.size() != colls.size()) {
        throw MapReduceException{};
    }
    
    // calculate the number of map rows and offsets
    decltype(filteredResults.size()) filteredRows = 0;
    decltype(filteredResults.size()) totalRows = 0;
    decltype(filteredResults.size()) offset = 0;
    std::vector<decltype(filteredRows)> filteredRowOffsets;
    for (const auto& result : filteredResults) {
        filteredRowOffsets.emplace_back(filteredRows);
        filteredRows += result->FilteredRows();
        totalRows += result->TotalRows();
        offset += result->Offset();
    }
    filteredRowOffsets.emplace_back(filteredRows);
    
    // allocate the results collection
    auto results = boost::make_shared<map_reduce_result_array_ptr::element_type>();
    results->reserve(filteredRows);
    
    // copy the shard results into the main results collection
    for (const auto& result : filteredResults) {
        results->insert(results->end(), result->cbegin(), result->cend(), result->SourceResults());
    }
    
    auto less = [](const map_reduce_result_ptr& a, const map_reduce_result_ptr& b) {
        return MapReduceResult::Less(a, b);
    };
    
    auto mergeResultsWorker = [&](decltype(collsSize) startIndex, decltype(collsSize) step) {
        BOOST_SCOPE_EXIT(&threads) { --threads; } BOOST_SCOPE_EXIT_END
        
        auto midStep = step / 2;
        auto midIndex = startIndex + midStep;
        auto endIndex = std::min(startIndex + step, collsSize);

        auto startOffset = filteredRowOffsets[startIndex];
        auto midOffset = filteredRowOffsets[midIndex];
        auto endOffset = filteredRowOffsets[endIndex];

        const auto& begin = results->begin();
        std::inplace_merge(begin + startOffset, begin + midOffset, begin + endOffset, less);
    };

    // merge the result shards on the main results collection
    decltype(collsSize) step = 2;
    const auto useThreadsForMerge = filteredRows >= 10000;
    while ((threads = collsSize / step) > 0) {
        
        for (decltype(collsSize) i = 0; i <= collsSize - step; i += step) {
            if (useThreadsForMerge) {
                mapReduceThreadPool_->Post([=](size_t) { mergeResultsWorker(i, step); });
            } else {
                mergeResultsWorker(i, step);
            }
        }
        
        // wait for the merge threads to finish
        while (threads.load() > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        step *= 2;
    }
    
    // if we had an odd number of collections then there is one left to merge
    if ((collsSize % step) > 0) {
        mergeResultsWorker(0, step);
    }
    
    return boost::make_shared<map_reduce_results_ptr::element_type>(results, offset, totalRows, skip, limit, descending);
}

map_reduce_result_array_ptr MapReduce::Execute(rs::jsapi::Runtime& rt, const MapReduceTask& task, const document_array& docs) {
    map_reduce_result_array_ptr results = boost::make_shared<map_reduce_result_array_ptr::element_type>();
    
    // create the function script
    std::string mapScript = "(function() { return ";
    mapScript += task.Map();
    mapScript += "; })();";
    
    document_array::value_type empty;
    auto doc = std::cref(empty);

    // define a function in global scope implemented by a C++ lambda
    rs::jsapi::Global::DefineFunction(rt, "emit", 
        [&](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value&) {
            auto source = ScriptArrayJsapiKeyValueSource::Create(args[0], args[1]);
            
            auto resultArr = rs::scriptobject::ScriptArrayFactory::CreateArray(source);
            auto result = MapReduceResult::Create(resultArr, doc);
            results->push_back(result);
    });

    // execute the script in the context of the runtime, getting the resulting function
    rs::jsapi::Value func(rt);
    rt.Evaluate(mapScript.c_str(), func);
    
    auto state = new MapReduceScriptObjectState{};

    rs::scriptobject::ScriptObjectPtr scriptObj;
    rs::jsapi::Value object(rt);
    rs::jsapi::DynamicObject::Create(rt, 
        [&](const char* name, rs::jsapi::Value& value) {
            return MapReduce::GetFieldValue(scriptObj, name, value);
        }, 
        nullptr, 
        [&](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>&) {
            // TODO: only enumerate when really needed
            for (decltype(scriptObj->getCount()) i = 0, count = scriptObj->getCount(); i < count; ++i) {
                props.emplace_back(scriptObj->getName(i));
            }
            return true;
        }, 
        [state]() { delete state; },
        object);

    rs::jsapi::DynamicObject::SetPrivate(object, 0, state);

    rs::jsapi::FunctionArguments args(rt);
    args.Append(object);

    for (DocumentCollection::size_type i = 0, size = docs.size(); i < size; ++i) {
        doc = std::cref(docs[i]);
        scriptObj = doc.get()->getObject();

        state->scriptObj_ = scriptObj;

        func.CallFunction(args, false);
    }
    
    SortResultArray(results);
    
    return results;
}

void MapReduce::GetFieldValue(script_object_ptr scriptObj, const char* name, rs::jsapi::Value& value) {
    int index = 0;
    switch (scriptObj->getType(name, index)) {
        case rs::scriptobject::ScriptObjectType::Boolean:
            value = scriptObj->getBoolean(index);
            return;
        case rs::scriptobject::ScriptObjectType::Int32:
            value = scriptObj->getInt32(index);
            return;
        case rs::scriptobject::ScriptObjectType::String:
            value = scriptObj->getString(index);
            return;
        case rs::scriptobject::ScriptObjectType::Double:
            value = scriptObj->getDouble(index);
            return;
        case rs::scriptobject::ScriptObjectType::Object: {
            auto childObj = scriptObj->getObject(index);
            MapReduce::CreateValueObject(childObj, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Array: {
            auto childArr = scriptObj->getArray(index);
            MapReduce::CreateValueArray(childArr, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Null:
            value = JS::NullHandleValue;
            return;
        default:
            value = JS::UndefinedHandleValue;
            return;
    }
}

void MapReduce::GetFieldValue(script_array_ptr scriptArr, int index, rs::jsapi::Value& value) {
    switch (scriptArr->getType(index)) {
        case rs::scriptobject::ScriptObjectType::Boolean:
            value = scriptArr->getBoolean(index);
            return;
        case rs::scriptobject::ScriptObjectType::Int32:
            value = scriptArr->getInt32(index);
            return;
        case rs::scriptobject::ScriptObjectType::String:
            value = scriptArr->getString(index);
            return;
        case rs::scriptobject::ScriptObjectType::Double:
            value = scriptArr->getDouble(index);
            return;
        case rs::scriptobject::ScriptObjectType::Object: {
            auto childObj = scriptArr->getObject(index);
            MapReduce::CreateValueObject(childObj, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Array: {
            auto childArr = scriptArr->getArray(index);
            MapReduce::CreateValueArray(childArr, value);
            return;
        }
        case rs::scriptobject::ScriptObjectType::Null:
            value = JS::NullHandleValue;
            return;
        default:
            value = JS::UndefinedHandleValue;
    }
}

void MapReduce::CreateValueObject(script_object_ptr obj, rs::jsapi::Value& value) {
    auto state = new MapReduceScriptObjectState{obj};
            
    auto cx = value.getContext();
    rs::jsapi::DynamicObject::Create(cx, 
        [state](const char* name, rs::jsapi::Value& value) {
            return MapReduce::GetFieldValue(state->scriptObj_, name, value);
        }, 
        nullptr, 
        [state](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>&) {
            // TODO: it would be useful to cache this for performance
            for (decltype(state->scriptObj_->getCount()) i = 0, count = state->scriptObj_->getCount(); i < count; ++i) {
                props.emplace_back(state->scriptObj_->getName(i));
            }
            return true;
        }, 
        [state]() { delete state; },
        value);

    rs::jsapi::DynamicObject::SetPrivate(value, 0, state);
}

void MapReduce::CreateValueArray(script_array_ptr arr, rs::jsapi::Value& value) {
    auto state = new MapReduceScriptArrayState{arr};
    
    auto cx = value.getContext();
    rs::jsapi::DynamicArray::Create(cx, 
        [state](int index, rs::jsapi::Value& value) {
            return MapReduce::GetFieldValue(state->scriptArray_, index, value);
        }, 
        nullptr, 
        [state]() { return state->scriptArray_->getCount(); }, 
        [state]() { delete state; },
        value);
        
    rs::jsapi::DynamicArray::SetPrivate(value, 0, state);
}

script_object_ptr MapReduce::GetValueScriptObject(const rs::jsapi::Value& obj) {
    if (rs::jsapi::DynamicObject::IsDynamicObject(obj)) {
        uint64_t value = 0;
        void* ptr = nullptr;
        rs::jsapi::DynamicObject::GetPrivate(obj, value, ptr);
        return reinterpret_cast<MapReduceScriptObjectState*>(ptr)->scriptObj_;
    }
    else {
        auto source = ScriptObjectJsapiSource::Create(obj);
        return rs::scriptobject::ScriptObjectFactory::CreateObject(source);
    }
}

script_array_ptr MapReduce::GetValueScriptArray(const rs::jsapi::Value& arr) {
    if (rs::jsapi::DynamicArray::IsDynamicArray(arr)) {
        uint64_t value = 0;
        void* ptr = nullptr;
        rs::jsapi::DynamicArray::GetPrivate(arr, value, ptr);
        return reinterpret_cast<MapReduceScriptArrayState*>(ptr)->scriptArray_;
    } else {
        auto source = ScriptArrayJsapiSource::Create(arr);
        return rs::scriptobject::ScriptArrayFactory::CreateArray(source);
    }
}

void MapReduce::SortResultArray(map_reduce_result_array_ptr results) {
    std::sort(results->begin(), results->end(), [](const map_reduce_result_ptr& a, const map_reduce_result_ptr& b) {
        return MapReduceResult::Less(a, b);
    });
}