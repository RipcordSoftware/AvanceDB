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

#include <ctime>

#include "../http_server_log.h"

class HttpServerLogTests : public ::testing::Test {
protected:
    virtual void SetUp() {

    }
    
    virtual void TearDown() {
        
    }
    
};

TEST_F(HttpServerLogTests, test0) {
    std::time_t time = 0;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1970, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(1, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);    
}

TEST_F(HttpServerLogTests, test1) {
    std::time_t time = 86400;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1970, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(2, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);    
}

TEST_F(HttpServerLogTests, test1b) {
    std::time_t time = 86407;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1970, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(2, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(7, sec);    
}

TEST_F(HttpServerLogTests, test1c) {
    std::time_t time = 86527;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1970, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(2, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(2, min);
    ASSERT_EQ(7, sec);    
}

TEST_F(HttpServerLogTests, test1d) {
    std::time_t time = 86527 + (8 * 3600);
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1970, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(2, day);
    ASSERT_EQ(8, hour);
    ASSERT_EQ(2, min);
    ASSERT_EQ(7, sec);    
}

TEST_F(HttpServerLogTests, test2) {
    // 1969-12-31
    std::time_t time = -86400;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    // we expect 1970-1-1
    ASSERT_EQ(1970, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(1, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);    
}

TEST_F(HttpServerLogTests, test3) {
    std::time_t time = 68083200;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1972, year);
    ASSERT_EQ(2, month);
    ASSERT_EQ(28, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);    
}

TEST_F(HttpServerLogTests, test4) {
    std::time_t time = 68169600;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(1972, year);
    ASSERT_EQ(2, month);
    ASSERT_EQ(29, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);    
}

TEST_F(HttpServerLogTests, test5) {
    std::time_t time = 1451606400;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(2016, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(1, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);
}

TEST_F(HttpServerLogTests, test6) {
    std::time_t time = 1483228800;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(2017, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(1, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);
}

TEST_F(HttpServerLogTests, test7) {
    std::time_t time = 1456617600;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(2016, year);
    ASSERT_EQ(2, month);
    ASSERT_EQ(28, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);
}

TEST_F(HttpServerLogTests, test8) {
    std::time_t time = 1456704000;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(2016, year);
    ASSERT_EQ(2, month);
    ASSERT_EQ(29, day);
    ASSERT_EQ(0, hour);
    ASSERT_EQ(0, min);
    ASSERT_EQ(0, sec);
}

TEST_F(HttpServerLogTests, test9) {
    std::time_t time = 1485192095;
    
    int year, month, day, hour, min, sec;    
    HttpServerLog::GetTimestamp(time, year, month, day, hour, min, sec);
    
    ASSERT_EQ(2017, year);
    ASSERT_EQ(1, month);
    ASSERT_EQ(23, day);
    ASSERT_EQ(17, hour);
    ASSERT_EQ(21, min);
    ASSERT_EQ(35, sec);
}