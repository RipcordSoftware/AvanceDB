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

#include "../json_helper.h"

#include <array>
#include <cstring>

class JsonHelperTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
};

TEST_F(JsonHelperTests, test0) {
    const char* json = "Hello world!";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(12, str.size());
    ASSERT_STREQ(json, str.c_str());
}

TEST_F(JsonHelperTests, test1) {
    const char* json = "\t\t\t\t";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('t', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('t', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('t', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('t', str[7]);
}

TEST_F(JsonHelperTests, test2) {
    const char* json = "\r\r\r\r";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('r', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('r', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('r', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('r', str[7]);
}

TEST_F(JsonHelperTests, test3) {
    const char* json = "\n\n\n\n";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('n', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('n', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('n', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('n', str[7]);
}

TEST_F(JsonHelperTests, test4) {
    const char* json = "\f\f\f\f";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('f', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('f', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('f', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('f', str[7]);
}

TEST_F(JsonHelperTests, test5) {
    const char* json = "\b\b\b\b";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('b', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('b', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('b', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('b', str[7]);
}

TEST_F(JsonHelperTests, test6) {
    const char* json = "\"\"\"\"";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('"', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('"', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('"', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('"', str[7]);
}

TEST_F(JsonHelperTests, test7) {
    const char* json = "\\\\\\\\";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('\\', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('\\', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('\\', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('\\', str[7]);
}

TEST_F(JsonHelperTests, test8) {
    const char* json = "////";
    auto str = JsonHelper::EscapeJsonString(json);
    ASSERT_EQ(8, str.size());
    ASSERT_EQ('\\', str[0]);
    ASSERT_EQ('/', str[1]);
    ASSERT_EQ('\\', str[2]);
    ASSERT_EQ('/', str[3]);
    ASSERT_EQ('\\', str[4]);
    ASSERT_EQ('/', str[5]);
    ASSERT_EQ('\\', str[6]);
    ASSERT_EQ('/', str[7]);    
}

TEST_F(JsonHelperTests, test9) {
    const char* json = "\t\n\r\f\b\"\\/";
    auto str = JsonHelper::EscapeJsonString(json);
    auto i = 0;
    ASSERT_EQ(16, str.size());
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('t', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('n', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('r', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('f', str[i++]);    
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('b', str[i++]);    
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('"', str[i++]);    
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('\\', str[i++]);    
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('/', str[i++]);    
}

TEST_F(JsonHelperTests, test10) {
    const char* json = "a\tb\nc\rd\fe\bf\"g\\/h";
    auto str = JsonHelper::EscapeJsonString(json);
    auto i = 0;
    ASSERT_EQ(24, str.size());
    ASSERT_EQ('a', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('t', str[i++]);
    ASSERT_EQ('b', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('n', str[i++]);
    ASSERT_EQ('c', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('r', str[i++]);
    ASSERT_EQ('d', str[i++]);
    ASSERT_EQ('\\', str[i++]);
    ASSERT_EQ('f', str[i++]);
    ASSERT_EQ('e', str[i++]);
}

TEST_F(JsonHelperTests, test11) {
    const char* json = "Hello world!";
    std::array<char, 1024> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, str.size());
    ASSERT_EQ(12, std::strlen(buffer.data()));
    ASSERT_STREQ(json, buffer.data());
}

TEST_F(JsonHelperTests, test12) {
    const char* json = "012345678901234567890123456789012345678901234567890123456789";
    std::array<char, 20> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(60, str.size());
    ASSERT_EQ(0, std::strlen(buffer.data()));
    ASSERT_STREQ(json, str.c_str());
}

TEST_F(JsonHelperTests, test13) {
    const char* json = "01234567890123456789";
    std::array<char, 20> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(20, str.size());
    ASSERT_EQ(0, std::strlen(buffer.data()));
    ASSERT_STREQ(json, str.c_str());
}

TEST_F(JsonHelperTests, test14) {
    const char* json = "01234567890123456789";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, str.size());
    ASSERT_EQ(20, std::strlen(buffer.data()));
    ASSERT_STREQ(json, buffer.data());
}

TEST_F(JsonHelperTests, test15) {
    const char* json = "\t012345678901234567";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, str.size());
    ASSERT_EQ(20, std::strlen(buffer.data()));
    ASSERT_EQ('\\', buffer.data()[0]);
    ASSERT_EQ('t', buffer.data()[1]);
    ASSERT_STREQ(json + 1, buffer.data() + 2);
}

TEST_F(JsonHelperTests, test16) {
    const char* json = "\t0123456789012345678";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(21, str.size());
    ASSERT_EQ(0, std::strlen(buffer.data()));
    ASSERT_EQ('\\', str.data()[0]);
    ASSERT_EQ('t', str.data()[1]);
    ASSERT_STREQ(json + 1, str.data() + 2);
}

TEST_F(JsonHelperTests, test17) {
    const char* json = "0123456789012345678\t";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, str.size());
    const auto len = std::strlen(buffer.data());
    ASSERT_EQ(21, len);
    ASSERT_EQ(0, std::strncmp(json, buffer.data(), len - 2));
    ASSERT_EQ('\\', buffer.data()[len - 2]);
    ASSERT_EQ('t', buffer.data()[len - 1]);
}

TEST_F(JsonHelperTests, test18) {
    const char* json = "0123456789012345678\t\n";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(23, str.size());
    ASSERT_EQ(0, std::strlen(buffer.data()));
    ASSERT_EQ(0, std::strncmp(json, str.data(), str.size() - 4));
    ASSERT_EQ('\\', str.data()[str.size() - 4]);
    ASSERT_EQ('t', str.data()[str.size() - 3]);
    ASSERT_EQ('\\', str.data()[str.size() - 2]);
    ASSERT_EQ('n', str.data()[str.size() - 1]);
}

TEST_F(JsonHelperTests, test19) {
    const char* json = "\t\n\r\"\\\t\n\r\"\\\t\n\r\"\\";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(30, str.size());
    ASSERT_EQ(0, std::strlen(buffer.data()));
    for (auto i = 0; i < 30; i += 10) {
        ASSERT_EQ(0, std::strncmp("\\t\\n\\r\\\"\\\\", str.data() + i, 10));
    }
}

TEST_F(JsonHelperTests, 20) {
    const char* json = "01234567890123456789";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, str.size());
    ASSERT_EQ(20, std::strlen(buffer.data()));
    ASSERT_STREQ(json, buffer.data());
    
    json = "0123456789012345678901234567890123456789";
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, std::strlen(buffer.data()));
    ASSERT_EQ(40, str.size());
    ASSERT_STREQ(json, str.data());
}

TEST_F(JsonHelperTests, 21) {
    const char* json = "0123456789012345678901234567890123456789";
    std::array<char, 22> buffer;
    std::string str;
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, std::strlen(buffer.data()));
    ASSERT_EQ(40, str.size());
    ASSERT_STREQ(json, str.data());
    
    json = "01234567890123456789";    
    JsonHelper::EscapeJsonString(json, buffer, str);
    ASSERT_EQ(0, str.size());
    ASSERT_EQ(20, std::strlen(buffer.data()));
    ASSERT_STREQ(json, buffer.data());    
}