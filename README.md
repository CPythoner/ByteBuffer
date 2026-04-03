# ByteBuffer

[![CI](https://github.com/CPythoner/ByteBuffer/actions/workflows/ci.yml/badge.svg)](https://github.com/CPythoner/ByteBuffer/actions/workflows/ci.yml)

C++ 实现的 Java NIO ByteBuffer 功能，支持跨平台（Windows、macOS、Linux）。

## 目录

- [ByteBuffer](#bytebuffer)
  - [目录](#目录)
  - [介绍](#介绍)
  - [特性](#特性)
  - [快速开始](#快速开始)
    - [创建 ByteBuffer](#创建-bytebuffer)
    - [写入数据](#写入数据)
    - [读取数据](#读取数据)
    - [状态切换](#状态切换)
  - [API 参考](#api-参考)
    - [创建方法](#创建方法)
    - [写入方法](#写入方法)
    - [读取方法](#读取方法)
    - [状态方法](#状态方法)
    - [其他方法](#其他方法)
  - [构建与测试](#构建与测试)
    - [依赖](#依赖)
    - [构建](#构建)
    - [运行测试](#运行测试)
    - [CI 状态](#ci-状态)
  - [注意事项](#注意事项)
  - [参考链接](#参考链接)

## 介绍

ByteBuffer 是一个字节缓存区，提供了一系列 put 和 get 方法，可以方便地将各种类型的数据存入缓存区或从缓存区读取数据。底层存储结构是字节数组，所有操作都基于该数组。

本实现参考了 Java NIO 的 ByteBuffer 类，使用 C++11 实现，支持跨平台使用。

## 特性

- ✅ 支持多种数据类型的读写（uint8_t、char、short、int、long、float、double）
- ✅ 自动扩容机制，无需担心缓冲区溢出
- ✅ 支持链式操作，所有 put 方法返回 `ByteBuffer&`
- ✅ 完整的状态管理（position、limit、mark、capacity）
- ✅ 移动语义支持，禁用拷贝防止资源泄漏
- ✅ 跨平台支持（Windows、macOS、Linux）
- ✅ 完整的 CI 测试覆盖

## 快速开始

### 创建 ByteBuffer

```cpp
#include "ByteBuffer.h"

// 创建默认大小（2048 字节）的 ByteBuffer
ByteBuffer bb1;

// 创建指定容量的 ByteBuffer
ByteBuffer bb2(1024, "myBuffer");

// 从现有数组创建 ByteBuffer
uint8_t data[] = {1, 2, 3, 4, 5};
ByteBuffer bb3(data, sizeof(data), "fromArray");
```

### 写入数据

```cpp
// 链式写入
bb.put(42)
  .putChar('A')
  .putShort(1000)
  .putInt(123456)
  .putFloat(3.14f)
  .putDouble(2.71828);

// 写入字节数组
uint8_t buf[] = {1, 2, 3, 4, 5};
bb.putBytes(buf, sizeof(buf));

// 在指定位置写入
bb.putInt(999, 0);  // 在索引 0 处写入
```

### 读取数据

```cpp
// 切换到读取模式
bb.flip();

// 顺序读取
uint8_t a = bb.get();
char c = bb.getChar();
uint16_t s = bb.getShort();
uint32_t i = bb.getInt();
float f = bb.getFloat();
double d = bb.getDouble();

// 从指定位置读取（不改变 position）
uint8_t x = bb.get(5);
uint32_t y = bb.getInt(10);

// 读取字节数组
uint8_t outBuf[10];
bb.getBytes(outBuf, 10);
bb.getBytes(5, outBuf, 5);  // 从索引 5 开始读取
```

### 状态切换

```cpp
// 写入状态 -> 读取状态
bb.flip();

// 重置为初始状态（可继续写入）
bb.clear();

// 标记当前位置
bb.mark();

// 回到标记位置
bb.reset();

// 回到起始位置
bb.rewind();

// 压缩缓冲区（删除已读数据）
bb.compact();
```

## API 参考

### 创建方法

| 方法 | 描述 |
|------|------|
| `ByteBuffer(capacity=2048, name="")` | 创建指定容量的 ByteBuffer |
| `ByteBuffer(arr, length, name="")` | 从字节数组创建 ByteBuffer |
| `~ByteBuffer()` | 析构函数，自动释放内存 |

### 写入方法

| 方法 | 描述 |
|------|------|
| `put(uint8_t value)` | 写入一个字节 |
| `put(uint8_t value, index)` | 在指定位置写入一个字节 |
| `putBytes(buf, len)` | 写入字节数组 |
| `putBytes(buf, len, index)` | 在指定位置写入字节数组 |
| `putChar(value)` | 写入 char |
| `putShort(value)` | 写入 uint16_t |
| `putInt(value)` | 写入 uint32_t |
| `putLong(value)` | 写入 uint64_t |
| `putFloat(value)` | 写入 float |
| `putDouble(value)` | 写入 double |

**注意**：所有 put 方法都返回 `ByteBuffer&`，支持链式调用。

### 读取方法

| 方法 | 描述 |
|------|------|
| `get()` | 读取一个字节 |
| `get(index)` | 从指定位置读取一个字节（const） |
| `getBytes(buf, len)` | 读取字节数组到缓冲区 |
| `getBytes(index, buf, len)` | 从指定位置读取字节数组（const） |
| `getChar()` / `getChar(index)` | 读取 char |
| `getShort()` / `getShort(index)` | 读取 uint16_t |
| `getInt()` / `getInt(index)` | 读取 uint32_t |
| `getLong()` / `getLong(index)` | 读取 uint64_t |
| `getFloat()` / `getFloat(index)` | 读取 float |
| `getDouble()` / `getDouble(index)` | 读取 double |

**注意**：带 `index` 参数的方法不会改变 `position` 的值。

### 状态方法

| 方法 | 描述 |
|------|------|
| `capacity()` | 返回缓冲区容量（const） |
| `position()` | 返回当前位置（const） |
| `position(newPos)` | 设置新位置 |
| `limit()` | 返回限制值（const） |
| `limit(newLimit)` | 设置新限制 |
| `flip()` | 切换到读取模式 |
| `clear()` | 重置为初始状态 |
| `mark()` | 标记当前位置 |
| `reset()` | 回到标记位置 |
| `rewind()` | 回到起始位置 |
| `compact()` | 压缩缓冲区 |
| `hasRemaining()` | 是否还有数据（const） |
| `remaining()` | 剩余字节数（const） |

### 其他方法

| 方法 | 描述 |
|------|------|
| `equals(other)` | 比较两个 ByteBuffer 是否相等 |
| `duplicate()` | 复制一个 ByteBuffer |
| `printInfo()` | 打印缓冲区信息 |

## 构建与测试

### 依赖

- CMake 3.10+
- C++11 兼容编译器

### 构建

```bash
mkdir build && cd build
cmake ..
make
```

### 运行测试

```bash
./ByteBuffer
```

或使用 CTest：

```bash
ctest --output-on-failure
```

### CI 状态

本项目使用 GitHub Actions 进行持续集成，支持以下平台：

| 平台 | Release | Debug |
|------|---------|-------|
| Ubuntu (ubuntu-latest) | ✅ | ✅ |
| macOS (macos-latest) | ✅ | ✅ |
| Windows (windows-latest) | ✅ | ✅ |

## 注意事项

1. **拷贝控制**：ByteBuffer 禁用了拷贝构造函数和拷贝赋值运算符，防止浅拷贝导致的双重释放问题。如需复制，请使用 `duplicate()` 方法或移动语义。

2. **内存对齐**：对于多字节类型的读写，某些架构（如 ARM）可能要求内存对齐。未对齐的访问可能导致性能下降或崩溃。建议在处理大量数据时确保数据对齐。

3. **字节序**：本实现使用主机字节序（host byte order）。如需网络传输，请自行处理字节序转换（如使用 `htons`/`ntohs` 等函数）。

4. **异常处理**：当内存分配失败时，会抛出 `std::bad_alloc` 异常。请确保在调用时进行适当的异常处理。

## 参考链接

- [Java ByteBuffer Documentation](https://docs.oracle.com/javase/7/docs/api/java/nio/ByteBuffer.html)
- [java.nio.Buffer 中的 flip() 方法](https://blog.csdn.net/hbtj_1216/article/details/53129588)
- [ByteBuffer 常用方法详解](https://blog.csdn.net/moakun/article/details/80630477)
- [Netty ByteBuf](https://netty.io/4.0/api/io/netty/buffer/ByteBuf.html)
