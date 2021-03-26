/**
 * @file ByteBufferCpp.cpp
 * @brief Test for ByteBuffer
 * @copyright Copyright (c) 2021 CPythoner. All Rights Reserved.
 * @author CPythoner (qiang0310@126.com)
 * @date 2021-03-25 16:41:53
 * @last_author: CPythoner (qiang0310@126.com)
 * @last_edit_time: 2021-03-25 23:36:35
 */

#include <string.h>

#include <math.h>
#include <iostream>
#include <iomanip>

#include "ByteBuffer.h"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

using namespace std;

TEST_CASE("ByteBuffer", "[ConstructorTest]")
{
        ByteBuffer bb;
        REQUIRE(bb.capacity() == DEFAULT_BUFFER_SIZE);
        REQUIRE(bb.limit()    == DEFAULT_BUFFER_SIZE);
        REQUIRE(bb.position() == 0);

        ByteBuffer bb2(10);
        REQUIRE(bb2.capacity() == 10);
        REQUIRE(bb2.limit()    == 10);
        REQUIRE(bb2.position() == 0);

        string     str = "hello bytebuffer";
        ByteBuffer bb3((uint8_t*)str.c_str(), static_cast<uint32_t>(str.size()));
        REQUIRE(bb3.capacity() == static_cast<uint32_t>(str.size()));
        REQUIRE(bb3.limit()    == static_cast<uint32_t>(str.size()));
        REQUIRE(bb3.position() == 0);
        uint8_t ret[32] = {0};
        bb3.getBytes(ret, static_cast<uint32_t>(str.size()));
        REQUIRE(strcmp((const char*)ret, str.c_str()) == 0);
}

TEST_CASE("ByteBuffer::CheckSize", "[CheckSize]")
{
    ByteBuffer bb(1);
    bb.put('5');
    REQUIRE(bb.capacity() == 1);
    bb.put(1);
    REQUIRE(bb.capacity() == 2049);
}

TEST_CASE("ByteBuffer::PutAndGet", "[PutAndGet]")
{
    std::string str = "hello bytebuffer";
    ByteBuffer bb;
    bb.put('5');
    bb.putShort(0x4567);
    bb.putInt(0xABCDEF98);
    bb.putLong(0x1234567890ABCDEF);
    bb.putFloat(2111.23f);
    bb.putDouble(35.5236245);
    bb.putBytes((uint8_t*)str.c_str(), str.size());
    bb.printInfo();

    bb.flip();
    bb.printInfo();

    REQUIRE(bb.get() == '5');
    REQUIRE(bb.getShort() == 0x4567);
    REQUIRE(bb.getInt() == 0xABCDEF98);
    REQUIRE(bb.getLong() == 0x1234567890ABCDEF);

    const double EPS = 1e-6;
    REQUIRE(fabs(bb.getFloat() - 2111.23f) <= EPS);
    REQUIRE(fabs(bb.getDouble() - 35.5236245) <= EPS);

    uint8_t retbytes[32] = {0};
    bb.getBytes(retbytes, str.size());
    REQUIRE(strcmp((char*)retbytes, str.c_str()) == 0);
}

TEST_CASE("ByteBuffer::flip", "[flip]")
{
    ByteBuffer bb(1);
    bb.putBytes((uint8_t*)"hello", 5);
    bb.flip();
    REQUIRE(bb.capacity() == 2049);
    REQUIRE(bb.limit() == 5);
    REQUIRE(bb.position() == 0);
}

TEST_CASE("ByteBuffer::compact", "[compact]")
{
    ByteBuffer bb;
    bb.putBytes((uint8_t*)"hello", 5);
    bb.flip();
    bb.get();
    bb.get(); // get two bytes, now position == 3
    bb.compact();
    REQUIRE(bb.limit() == bb.capacity());
    REQUIRE(bb.position() == 3);
}
