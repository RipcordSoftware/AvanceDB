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

#include <gtest/gtest.h>

#include <vector>
#include <cstring>
#include <memory>

#include <boost/format.hpp>

#include "libscriptobject_gason.h"
#include "script_object_factory.h"
#include "script_array_factory.h"
#include "script_array_vector_source.h"

#include "libhttpserver.h"

#include "../databases.h"
#include "../database.h"
#include "../rest_exceptions.h"
#include "../get_view_options.h"
#include "../config.h"
#include "../map_reduce_thread_pool.h"
#include "../map_reduce_result.h"
#include "../map_reduce_result_comparers.h"

class MapReduceTests : public ::testing::Test {
protected:
    MapReduceTests() {

    }
    
    static void SetUpTestCase() {
        threadPool_.reset(new MapReduceThreadPoolScope{Config::SpiderMonkey::GetHeapSize(), Config::SpiderMonkey::GetEnableBaselineCompiler(), Config::SpiderMonkey::GetEnableIonCompiler()});
        
        auto dbName = "MapReduceTests";
        databases_.AddDatabase(dbName);
        db_ = databases_.GetDatabase(dbName);
        
        std::string json = R"({"docs":[)";
        for (auto i = 0; i < 1000; ++i) {
            if (i > 0) {
                json += ',';
            }
            
            auto id = MakeDocId(i);
            json += MakeDocJson(id, i);
        }
        json += R"(]})";
        
        std::vector<char> buffer{json.cbegin(), json.cend()};
        buffer.push_back('\0');
        
        rs::scriptobject::ScriptObjectJsonSource source(buffer.data());        
        auto obj = rs::scriptobject::ScriptObjectFactory::CreateObject(source, false);
        docs_ = obj->getArray("docs");
    }
    
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
    static std::string MakeDocId(unsigned id) {
        return (boost::format("%08u") % id).str();
    }
    
    static std::string MakeDocJson(const std::string& id, unsigned index) {
        return MakeDocJson(id.c_str(), index);
    }
    
    static std::string MakeDocJson(const char* id, unsigned index) {
        auto json = (boost::format(R"({"_id":"%s","num":42,"sunny":true,"pi":3.14159,"lorem":"ipsum","obj":{},"arr":[],"index":%u})") % id % index).str();
        return json;
    }   
    
    static rs::scriptobject::ScriptObjectPtr MakeMapObject(const char* mapFunc, const char* reduceFunc = nullptr, const char* language = nullptr) {
        std::string json{R"({"map":")"};
        json += mapFunc;
        json += '"';
        if (reduceFunc) {
            json += R"(,"reduce":")";
            json += reduceFunc;
            json += '"';
        }
        if (language) {
            json += R"(,"language":")";
            json += language;
            json += '"';
        }
        json += '}';
        std::vector<char> buffer{json.cbegin(), json.cend()};
        buffer.push_back('\0');
        
        rs::scriptobject::ScriptObjectJsonSource source(buffer.data());        
        auto obj = rs::scriptobject::ScriptObjectFactory::CreateObject(source, false);
        return obj;
    }
    
    static void CompareResults(map_reduce_result_array_ptr::element_type::const_iterator begin, map_reduce_result_array_ptr::element_type::const_iterator end,
            map_reduce_result_array_ptr::element_type::const_reverse_iterator riter, map_reduce_result_array_ptr::element_type::const_reverse_iterator rend) {
        map_reduce_result_array_ptr::element_type items{begin, end};
        std::reverse(items.begin(), items.end());

        auto iter = items.cbegin();
        auto iterEnd = items.cend();
        for (; iter != iterEnd && riter != rend ; ++iter, ++riter) {
            auto diff = MapReduceResultComparers::Compare(*iter, *riter);
            ASSERT_EQ(0, diff);
        }
    }
    
    static Databases databases_;
    static database_ptr db_;
    static script_array_ptr docs_;
    
    static std::unique_ptr<MapReduceThreadPoolScope> threadPool_;
};

Databases MapReduceTests::databases_;
database_ptr MapReduceTests::db_;
script_array_ptr MapReduceTests::docs_;
std::unique_ptr<MapReduceThreadPoolScope> MapReduceTests::threadPool_;

TEST_F(MapReduceTests, test0) {
    ASSERT_EQ(0, db_->DocCount());
    ASSERT_EQ(0, db_->DocDelCount());
    ASSERT_EQ(0, db_->DiskSize());    
    ASSERT_EQ(0, db_->DataSize());
    ASSERT_EQ(0, db_->UpdateSequence());
    ASSERT_EQ(0, db_->CommitedUpdateSequence());
    ASSERT_EQ(0, db_->PurgeSequence());
    
    auto results = db_->PostBulkDocuments(docs_, true);
    ASSERT_EQ(1000, db_->DocCount());
    ASSERT_EQ(1000, results.size());
}

TEST_F(MapReduceTests, test1) {
    rs::httpserver::QueryString qs{""};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(results->TotalRows(), std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto obj = docs_->getObject(i);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(i, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(results->TotalRows(), std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test2) {
    rs::httpserver::QueryString qs{""};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount() * 2, results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(results->TotalRows(), std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = i / 2;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(results->TotalRows(), std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - (i / 2);
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test3) {
    rs::httpserver::QueryString qs{""};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { if (doc.index % 2 == 0) emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount() / 2, results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(results->TotalRows(), std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, i += 2) {
        const auto& result = *iter;
        auto obj = docs_->getObject(i);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(i, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(results->TotalRows(), std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, i += 2) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 2 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test4) {
    rs::httpserver::QueryString qs{"skip=10"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(results->TotalRows() - 10, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto obj = docs_->getObject(i + 10);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(i + 10, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(results->TotalRows() - 10, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test4b) {
    rs::httpserver::QueryString qs{"skip=100000"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1000, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));
}

TEST_F(MapReduceTests, test5) {
    rs::httpserver::QueryString qs{"skip=10&limit=20"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto obj = docs_->getObject(i + 10);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(i + 10, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 10 + 20 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test6) {
    rs::httpserver::QueryString qs{"descending=true"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(results->TotalRows(), std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto obj = docs_->getObject(i);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(i, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(results->TotalRows(), std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test7) {
    rs::httpserver::QueryString qs{"descending=true&skip=10"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(results->TotalRows() - 10, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto obj = docs_->getObject(i);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(i, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(results->TotalRows() - 10, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 10 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test7b) {
    rs::httpserver::QueryString qs{"descending=true&skip=100000"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(docs_->getCount(), results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));    
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));    
}

TEST_F(MapReduceTests, test8) {
    rs::httpserver::QueryString qs{"descending=true&skip=10&limit=20"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = docs_->getCount() - 10 - 20 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 10 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test9) {
    rs::httpserver::QueryString qs{R"(startkey=900)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(900, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(100, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 900 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(100, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test10) {
    rs::httpserver::QueryString qs{R"(startkey=900&skip=10)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(910, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(90, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 910 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(90, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test11) {
    rs::httpserver::QueryString qs{R"(startkey=900&skip=1000)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1000, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));
}

TEST_F(MapReduceTests, test12) {
    rs::httpserver::QueryString qs{R"(startkey=900&skip=10&limit=20)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(910, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 910 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 + 10 + 20 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test13) {
    rs::httpserver::QueryString qs{R"(startkey=900&skip=10&limit=200)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(910, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(90, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 910 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(90, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = docs_->getCount() - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test14) {
    rs::httpserver::QueryString qs{R"(endkey=100)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(101, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(101, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 101 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test15) {
    rs::httpserver::QueryString qs{R"(endkey=100&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(100, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(100, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 100 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test16) {
    rs::httpserver::QueryString qs{R"(endkey=100&skip=10)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(91, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 10 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(91, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 101 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test17) {
    rs::httpserver::QueryString qs{R"(endkey=100&inclusive_end=false&skip=10)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(90, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 10 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(90, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 100 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test18) {
    rs::httpserver::QueryString qs{R"(endkey=100&skip=10&limit=20)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(10, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 10 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 10 + 20 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test19) {
    rs::httpserver::QueryString qs{R"(startkey=800&endkey=900)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(800, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(101, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 800 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(101, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 901 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test20) {
    rs::httpserver::QueryString qs{R"(startkey=800&endkey=900&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(800, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(100, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 800 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(100, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test21) {
    rs::httpserver::QueryString qs{R"(startkey=800&endkey=900&skip=10)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(810, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(91, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 810 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(91, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 901 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test22) {
    rs::httpserver::QueryString qs{R"(startkey=800&endkey=900&limit=10)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(800, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(10, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 800 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(10, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 800 + 10 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test23) {
    rs::httpserver::QueryString qs{R"(startkey=800&endkey=900&skip=10&limit=20)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(810, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 810 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 800 + 10 + 20 - 1 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test24) {
    rs::httpserver::QueryString qs{R"(startkey=800&endkey=900&skip=1000&limit=200)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(901, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));
}

TEST_F(MapReduceTests, test25) {
    rs::httpserver::QueryString qs{R"(startkey=900&endkey=800&descending=true)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(99, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(101, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 800 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(101, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test26) {
    rs::httpserver::QueryString qs{R"(startkey=900&endkey=800&descending=true&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(99, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(100, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 801 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(100, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test27) {
    rs::httpserver::QueryString qs{R"(startkey=900&endkey=800&descending=true&skip=10)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(109, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(91, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 800 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(91, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - 10 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test28) {
    rs::httpserver::QueryString qs{R"(startkey=900&endkey=800&descending=true&skip=10&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(109, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(90, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 801 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(90, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - 10 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test29) {
    rs::httpserver::QueryString qs{R"(startkey=900&endkey=800&descending=true&skip=10&limit=20)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(109, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 900 - 10 - 20 + 1 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - 10 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test30) {
    rs::httpserver::QueryString qs{R"(startkey=900&endkey=800&descending=true&skip=10&limit=20&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(109, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(20, std::distance(iter, end));
    
    for (auto i = 0; iter != end; ++iter, ++i) {
        const auto& result = *iter;
        auto index = 900 - 10 - 20 + 1 + i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(20, std::distance(riter, rend));
    
    for (auto i = 0; riter != rend; ++riter, ++i) {
        const auto& result = *riter;
        auto index = 900 - 10 - i;
        auto obj = docs_->getObject(index);
        ASSERT_STREQ(obj->getString("_id"), result->getId());
        ASSERT_EQ(index, result->getKeyDouble());
        ASSERT_EQ(rs::scriptobject::ScriptObjectType::Null, result->getValueType());
    }
    
    CompareResults(results->cbegin(), results->cend(), results->crbegin(), results->crend());
}

TEST_F(MapReduceTests, test31) {
    rs::httpserver::QueryString qs{R"(startkey=1000)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1000, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));   
}

TEST_F(MapReduceTests, test32) {
    rs::httpserver::QueryString qs{R"(endkey=0)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(1, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(1, std::distance(riter, rend));   
}

TEST_F(MapReduceTests, test33) {
    rs::httpserver::QueryString qs{R"(endkey=0&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));   
}

TEST_F(MapReduceTests, test34) {
    rs::httpserver::QueryString qs{R"(startkey=1000&endkey=0&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1000, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));   
}

TEST_F(MapReduceTests, test35) {
    rs::httpserver::QueryString qs{R"(startkey=1000&endkey=0&inclusive_end=false&skip=2000)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1000, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));   
}

TEST_F(MapReduceTests, test36) {
    rs::httpserver::QueryString qs{R"(startkey=0&endkey=1000&descending=true)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(999, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));   
}

TEST_F(MapReduceTests, test37) {
    rs::httpserver::QueryString qs{R"(startkey=0&endkey=1000&descending=true&inclusive_end=false)"};
    GetViewOptions options{qs};
    
    auto mapObj = MakeMapObject(R"(function(doc) { emit(doc.index, null); })");
    auto results = db_->PostTempView(options, mapObj);
    
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(999, results->Offset());
    ASSERT_EQ(docs_->getCount(), results->TotalRows());
    
    auto iter = results->cbegin();
    auto end = results->cend();       
    ASSERT_EQ(0, std::distance(iter, end));
    
    auto riter = results->crbegin();
    auto rend = results->crend();
    ASSERT_EQ(0, std::distance(riter, rend));   
}