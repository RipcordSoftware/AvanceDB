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

#include <gtest/gtest.h>

#include <vector>
#include <cmath>

#include "../config.h"

class ConfigTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        Config::Clear();
    }
    
    virtual void TearDown() {
        
    }
    
};

TEST_F(ConfigTests, test0) {
    const char* args[] = { nullptr };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_TRUE(!Config::IsHelp());
    ASSERT_TRUE(!Config::IsDaemon());
    ASSERT_STREQ(Config::Http::DefaultAddress, Config::Http::Address().c_str());
    ASSERT_EQ(Config::Http::DefaultPort, Config::Http::Port());
    ASSERT_EQ(Config::Http::DefaultWorkersPerCpu, Config::Http::WorkersPerCpu());
    ASSERT_EQ(std::lround(Config::Http::DefaultWorkersPerCpu * Config::Environment::CpuCount()), Config::Http::Workers());
    ASSERT_FALSE(Config::Process::Color());
    ASSERT_EQ(0, Config::Process::PidFile().size());
    ASSERT_EQ(0, Config::Process::StdOutFile().size());
    ASSERT_EQ(0, Config::Process::StdErrFile().size());
    ASSERT_EQ(0, Config::Process::RootDirectory().size());
    ASSERT_EQ(Config::Environment::RealCpuCount(), Config::Environment::CpuCount());
    ASSERT_EQ(Config::SpiderMonkey::DefaultHeapSizeMB * 1024 * 1024, Config::SpiderMonkey::HeapSize());
    ASSERT_EQ(Config::SpiderMonkey::DefaultNurserySizeMB * 1024 * 1024, Config::SpiderMonkey::NurserySize());
    ASSERT_TRUE(Config::SpiderMonkey::EnableBaselineCompiler());
    ASSERT_TRUE(Config::SpiderMonkey::EnableIonCompiler());
}

TEST_F(ConfigTests, test1) {
    const char* args[] = { nullptr, "-h" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_TRUE(Config::IsHelp());
}

TEST_F(ConfigTests, test2) {
    const char* args[] = { nullptr, "-a", "127.0.0.1" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("127.0.0.1", Config::Http::Address().c_str());
}

TEST_F(ConfigTests, test3) {
    const char* args[] = { nullptr, "--address", "10.1.2.3" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("10.1.2.3", Config::Http::Address().c_str());
}

TEST_F(ConfigTests, test4) {
    const char* args[] = { nullptr, "-p", "666" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(666, Config::Http::Port());
}

TEST_F(ConfigTests, test5) {
    const char* args[] = { nullptr, "--port", "8080" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(8080, Config::Http::Port());
}

// REMOVED
//TEST_F(ConfigTests, test6) {
//    const char* args[] = { nullptr, "-d" };
//    
//    Config::Parse(sizeof(args) / sizeof(args[0]), args);
//    
//    ASSERT_TRUE(Config::IsDaemon());
//}

TEST_F(ConfigTests, test7) {
    const char* args[] = { nullptr, "--daemon" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_TRUE(Config::IsDaemon());
}

TEST_F(ConfigTests, test8) {
    const char* args[] = { nullptr, "-o", "outfile" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("outfile", Config::Process::StdOutFile().c_str());
}

TEST_F(ConfigTests, test9) {
    const char* args[] = { nullptr, "--out", "out.txt" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("out.txt", Config::Process::StdOutFile().c_str());
}

TEST_F(ConfigTests, test10) {
    const char* args[] = { nullptr, "-e", "errfile" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("errfile", Config::Process::StdErrFile().c_str());
}

TEST_F(ConfigTests, test11) {
    const char* args[] = { nullptr, "--err", "err.txt" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("err.txt", Config::Process::StdErrFile().c_str());
}

TEST_F(ConfigTests, test12) {
    const char* args[] = { nullptr, "--pid", "pid.txt" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("pid.txt", Config::Process::PidFile().c_str());
}

TEST_F(ConfigTests, test13) {
    const char* args[] = { nullptr, "--dir", "~/webroot" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_STREQ("~/webroot", Config::Process::RootDirectory().c_str());
}

TEST_F(ConfigTests, test14) {
    const char* args[] = { nullptr, "--color" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_TRUE(Config::Process::Color());
}

TEST_F(ConfigTests, test15) {
    const char* args[] = { nullptr, "--jsapi-heap-size", "256" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(256 * 1024 * 1024, Config::SpiderMonkey::HeapSize());
}

TEST_F(ConfigTests, test16) {
    const char* args[] = { nullptr, "--jsapi-nursery-size", "32" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(32 * 1024 * 1024, Config::SpiderMonkey::NurserySize());
}

TEST_F(ConfigTests, test17) {
    const char* args[] = { nullptr, "--jsapi-disable-baseline" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_FALSE(Config::SpiderMonkey::EnableBaselineCompiler());
}

TEST_F(ConfigTests, test18) {
    const char* args[] = { nullptr, "--jsapi-disable-ion" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_FALSE(Config::SpiderMonkey::EnableIonCompiler());
}

TEST_F(ConfigTests, test19) {
    const char* args[] = { nullptr, "--cpus", "3" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(3, Config::Environment::CpuCount());
}

TEST_F(ConfigTests, test20) {
    const char* args[] = { nullptr, "--http-workers", "4" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(4, Config::Http::WorkersPerCpu());
    ASSERT_EQ(4 * Config::Environment::RealCpuCount(), Config::Http::Workers());
}

TEST_F(ConfigTests, test21) {
    const char* args[] = { nullptr, "--http-workers", "4", "--cpus", "8" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(4, Config::Http::WorkersPerCpu());
    ASSERT_EQ(8, Config::Environment::CpuCount());
    ASSERT_EQ(4 * 8, Config::Http::Workers());
}

TEST_F(ConfigTests, test22) {
    const char* args[] = { nullptr, "--mapreduce-workers", "4" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(4.0, Config::MapReduce::WorkersPerCpu());
    ASSERT_EQ(std::lround(4.0 * Config::Environment::CpuCount()), Config::MapReduce::Workers());
}

TEST_F(ConfigTests, test23) {
    const char* args[] = { nullptr, "--mapreduce-workers", "4", "--cpus", "8" };
    
    Config::Parse(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(4.0, Config::MapReduce::WorkersPerCpu());
    ASSERT_EQ(8, Config::Environment::CpuCount());
    ASSERT_EQ(std::lround(4.0 * Config::Environment::CpuCount()), Config::MapReduce::Workers());
}
