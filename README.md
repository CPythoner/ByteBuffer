# ByteBuffer

[![CI](https://github.com/CPythoner/ByteBuffer/actions/workflows/ci.yml/badge.svg)](https://github.com/CPythoner/ByteBuffer/actions/workflows/ci.yml)

A C++ implementation of Java NIO ByteBuffer, supporting cross-platform use (Windows, macOS, Linux).

> **Header-Only**: Simply include `ByteBuffer.h` to use, no need to compile or link additional library files.

---

**🌐 Language:** [English](README.md) | [中文](README_CN.md)

## Table of Contents

- [ByteBuffer](#bytebuffer)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Quick Start](#quick-start)
    - [Creating ByteBuffer](#creating-bytebuffer)
    - [Writing Data](#writing-data)
    - [Reading Data](#reading-data)
    - [State Transitions](#state-transitions)
  - [API Reference](#api-reference)
    - [Creation Methods](#creation-methods)
    - [Write Methods](#write-methods)
    - [Read Methods](#read-methods)
    - [State Methods](#state-methods)
    - [Other Methods](#other-methods)
    - [Java Compatibility APIs](#java-compatibility-apis)
      - [Array Access Methods](#array-access-methods)
      - [Direct Buffer Methods](#direct-buffer-methods)
      - [Byte Order Methods](#byte-order-methods)
      - [Comparison and Utility Methods](#comparison-and-utility-methods)
  - [Build and Test](#build-and-test)
    - [Requirements](#requirements)
    - [Usage (Header-Only)](#usage-header-only)
    - [Build (For Testing Only)](#build-for-testing-only)
    - [CI Status](#ci-status)
  - [Caveats](#caveats)
  - [References](#references)

## Introduction

ByteBuffer is a byte buffer that provides a series of put and get methods for easily storing and retrieving various types of data. The underlying storage structure is a byte array, and all operations are based on this array.

This implementation is based on Java NIO's ByteBuffer class, implemented in C++11 for cross-platform use.

## Features

- ✅ **Header-Only**: Simply include `ByteBuffer.h` to use
- ✅ Supports reading and writing of multiple data types (uint8_t, char, short, int, long, float, double)
- ✅ Automatic expansion mechanism, no need to worry about buffer overflow
- ✅ Supports chained operations, all put methods return `ByteBuffer&`
- ✅ Complete state management (position, limit, mark, capacity)
- ✅ Move semantics support, copy disabled to prevent resource leaks
- ✅ Cross-platform support (Windows, macOS, Linux)
- ✅ Complete CI test coverage

## Quick Start

### Creating ByteBuffer

```cpp
#include "ByteBuffer.h"

// Create default size (2048 bytes) ByteBuffer
ByteBuffer bb1;

// Create ByteBuffer with specified capacity
ByteBuffer bb2(1024, "myBuffer");

// Create ByteBuffer from existing array
uint8_t data[] = {1, 2, 3, 4, 5};
ByteBuffer bb3(data, sizeof(data), "fromArray");
```

### Writing Data

```cpp
// Chain writing
bb.put(42)
  .putChar('A')
  .putShort(1000)
  .putInt(123456)
  .putFloat(3.14f)
  .putDouble(2.71828);

// Write byte array
uint8_t buf[] = {1, 2, 3, 4, 5};
bb.putBytes(buf, sizeof(buf));

// Write at specified position
bb.putInt(999, 0);  // Write at index 0
```

### Reading Data

```cpp
// Switch to read mode
bb.flip();

// Sequential reading
uint8_t a = bb.get();
char c = bb.getChar();
uint16_t s = bb.getShort();
uint32_t i = bb.getInt();
float f = bb.getFloat();
double d = bb.getDouble();

// Read from specified position (does not change position)
uint8_t x = bb.get(5);
uint32_t y = bb.getInt(10);

// Read byte array
uint8_t outBuf[10];
bb.getBytes(outBuf, 10);
bb.getBytes(5, outBuf, 5);  // Read starting from index 5
```

### State Transitions

```cpp
// Write state -> Read state
bb.flip();

// Reset to initial state (can continue writing)
bb.clear();

// Mark current position
bb.mark();

// Return to marked position
bb.reset();

// Return to starting position
bb.rewind();

// Compact buffer (remove read data)
bb.compact();
```

## API Reference

### Creation Methods

| Method | Description |
|--------|-------------|
| `ByteBuffer(capacity=2048, name="")` | Create ByteBuffer with specified capacity |
| `ByteBuffer(arr, length, name="")` | Create ByteBuffer from byte array |
| `~ByteBuffer()` | Destructor, automatically frees memory |

### Write Methods

| Method | Description |
|--------|-------------|
| `put(uint8_t value)` | Write a byte |
| `put(uint8_t value, index)` | Write a byte at specified position |
| `putBytes(buf, len)` | Write byte array |
| `putBytes(buf, len, index)` | Write byte array at specified position |
| `putChar(value)` | Write char |
| `putShort(value)` | Write uint16_t |
| `putInt(value)` | Write uint32_t |
| `putLong(value)` | Write uint64_t |
| `putFloat(value)` | Write float |
| `putDouble(value)` | Write double |

**Note**: All put methods return `ByteBuffer&`, supporting chained calls.

### Read Methods

| Method | Description |
|--------|-------------|
| `get()` | Read a byte |
| `get(index)` | Read a byte from specified position (const) |
| `getBytes(buf, len)` | Read byte array to buffer |
| `getBytes(index, buf, len)` | Read byte array from specified position (const) |
| `getChar()` / `getChar(index)` | Read char |
| `getShort()` / `getShort(index)` | Read uint16_t |
| `getInt()` / `getInt(index)` | Read uint32_t |
| `getLong()` / `getLong(index)` | Read uint64_t |
| `getFloat()` / `getFloat(index)` | Read float |
| `getDouble()` / `getDouble(index)` | Read double |

**Note**: Methods with `index` parameter do not change the `position` value.

### State Methods

| Method | Description |
|--------|-------------|
| `capacity()` | Return buffer capacity (const) |
| `position()` | Return current position (const) |
| `position(newPos)` | Set new position |
| `limit()` | Return limit value (const) |
| `limit(newLimit)` | Set new limit |
| `flip()` | Switch to read mode |
| `clear()` | Reset to initial state |
| `mark()` | Mark current position |
| `reset()` | Return to marked position |
| `rewind()` | Return to starting position |
| `compact()` | Compact buffer |
| `hasRemaining()` | Check if data remains (const) |
| `remaining()` | Return remaining bytes (const) |

### Other Methods

| Method | Description |
|--------|-------------|
| `equals(other)` | Compare if two ByteBuffers are equal |
| `duplicate()` | Duplicate a ByteBuffer |
| `printInfo()` | Print buffer information |

### Java Compatibility APIs

#### Array Access Methods

| Method | Description |
|--------|-------------|
| `hasArray()` | Check if array is accessible (always returns true) |
| `array()` | Return underlying byte array pointer |
| `const array() const` | Return underlying byte array pointer (const version) |
| `arrayOffset()` | Return array offset (always returns 0) |

#### Direct Buffer Methods

| Method | Description |
|--------|-------------|
| `isDirect()` | Check if direct buffer (currently returns false) |

#### Byte Order Methods

| Method | Description |
|--------|-------------|
| `ByteOrder` | Enum type: `ORDER_BIG_ENDIAN` / `ORDER_LITTLE_ENDIAN` |
| `order()` | Get current byte order |
| `order(ByteOrder)` | Set byte order, returns `ByteBuffer&` for chaining |
| `static nativeOrder()` | Get host byte order |

**Byte Order Usage Example:**

```cpp
#include "ByteBuffer.h"

ByteBuffer bb;

// Set to big-endian byte order (network byte order)
bb.order(ByteOrder::ORDER_BIG_ENDIAN);

// Write data (automatically stored in big-endian order)
bb.putInt(0x12345678);  // Byte order in buffer: 12 34 56 78

// Switch to little-endian byte order
bb.order(ByteOrder::ORDER_LITTLE_ENDIAN);
bb.putInt(0x12345678);  // Byte order in buffer: 78 56 34 12

// Automatic conversion when reading
bb.flip();
bb.order(ByteOrder::ORDER_BIG_ENDIAN);
uint32_t value = bb.getInt();  // Correctly reads 0x12345678
```

#### Comparison and Utility Methods

| Method | Description |
|--------|-------------|
| `compareTo(const ByteBuffer&)` | Compare two buffers lexicographically, returns negative/0/positive |
| `hash()` | Calculate hash value of buffer content |
| `toString()` | Return string representation of buffer |

**Usage Example:**

```cpp
// Compare buffers
ByteBuffer bb1, bb2;
// ... fill data ...
int result = bb1.compareTo(bb2);  // <0: bb1<bb2, =0: equal, >0: bb1>bb2

// Hash value (can be used in unordered_map, etc.)
size_t h = bb1.hash();

// String representation
std::string str = bb1.toString();  // "ByteBuffer[pos=5 lim=10 cap=100]"
```

## Build and Test

### Requirements

- C++11 compatible compiler
- CMake 3.10+ (for building tests only)

### Usage (Header-Only)

Since this is a header-only library, simply copy `ByteBuffer.h` to your project to use:

```cpp
#include "ByteBuffer.h"

int main() {
    ByteBuffer bb;
    bb.putInt(12345);
    return 0;
}
```

**No need to compile or link additional library files** since all implementations are in the header file.

### Build (For Testing Only)

If you want to run tests:

```bash
mkdir build && cd build
cmake ..
make
./ByteBuffer
```

Or use CTest:

```bash
ctest --output-on-failure
```

### CI Status

This project uses GitHub Actions for continuous integration, supporting the following platforms:

| Platform | Release | Debug |
|----------|---------|-------|
| Ubuntu (ubuntu-latest) | ✅ | ✅ |
| macOS (macos-latest) | ✅ | ✅ |
| Windows (windows-latest) | ✅ | ✅ |

## Caveats

1. **Copy Control**: ByteBuffer disables copy constructor and copy assignment operator to prevent double-free issues caused by shallow copy. If you need to copy, please use the `duplicate()` method or move semantics.

2. **Memory Alignment**: For multi-byte type read/write operations, some architectures (such as ARM) may require memory alignment. Unaligned access may cause performance degradation or crashes. It is recommended to ensure data alignment when handling large amounts of data.

3. **Byte Order**: This implementation supports byte order settings and defaults to host byte order. You can set big-endian or little-endian byte order through the `order()` method, which is suitable for cross-platform data serialization and network transmission.

4. **Exception Handling**: When memory allocation fails, a `std::bad_alloc` exception will be thrown. Please ensure proper exception handling when calling.

## References

- [Java ByteBuffer Documentation](https://docs.oracle.com/javase/7/docs/api/java/nio/ByteBuffer.html)
- [Netty ByteBuf](https://netty.io/4.0/api/io/netty/buffer/ByteBuf.html)
