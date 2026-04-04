# ByteBuffer

[![CI](https://github.com/CPythoner/ByteBuffer/actions/workflows/ci.yml/badge.svg)](https://github.com/CPythoner/ByteBuffer/actions/workflows/ci.yml)

C++ 实现的 Java NIO ByteBuffer 功能，支持跨平台（Windows、macOS、Linux）。

> **Header-Only**：只需包含 `ByteBuffer.h` 即可使用，无需编译链接额外的库文件。

---

**🌐 语言:** [English](README.md) | [中文](README_CN.md)

## 目录

- [ByteBuffer](#bytebuffer)
  - [目录](#目录)
  - [简介](#简介)
  - [特性](#特性)
  - [快速开始](#快速开始)
    - [创建 ByteBuffer](#创建-bytebuffer)
    - [写入数据](#写入数据)
    - [读取数据](#读取数据)
    - [状态转换](#状态转换)
  - [API 参考](#api-参考)
    - [创建方法](#创建方法)
    - [写入方法](#写入方法)
    - [读取方法](#读取方法)
    - [状态方法](#状态方法)
    - [其他方法](#其他方法)
    - [Java 兼容接口](#java-兼容接口)
      - [数组访问方法](#数组访问方法)
      - [直接缓冲区方法](#直接缓冲区方法)
      - [字节序方法](#字节序方法)
      - [比较和辅助方法](#比较和辅助方法)
  - [构建与测试](#构建与测试)
    - [依赖](#依赖)
    - [使用方法（Header-Only）](#使用方法header-only)
    - [构建（仅用于测试）](#构建仅用于测试)
    - [CI 状态](#ci-状态)
  - [注意事项](#注意事项)
  - [参考链接](#参考链接)

## 简介

ByteBuffer 是一个字节缓冲区，通过提供一系列的 put 和 get 方法，可以方便地存储和检索各种类型的数据。底层存储结构是字节数组，所有操作都基于这个数组进行。

本实现基于 Java NIO 的 ByteBuffer 类，使用 C++11 实现，支持跨平台使用。

## 特性

- ✅ **Header-Only**：只需包含 `ByteBuffer.h` 即可使用
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
// 切换到读模式
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

### 状态转换

```cpp
// 写状态 -> 读状态
bb.flip();

// 重置为初始状态（可以继续写入）
bb.clear();

// 标记当前位置
bb.mark();

// 返回标记位置
bb.reset();

// 返回起始位置
bb.rewind();

// 压缩缓冲区（移除已读数据）
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

**注意**：带 `index` 参数的方法不会改变 `position` 值。

### 状态方法

| 方法 | 描述 |
|------|------|
| `capacity()` | 返回缓冲区容量（const） |
| `position()` | 返回当前位置（const） |
| `position(newPos)` | 设置新位置 |
| `limit()` | 返回 limit 值（const） |
| `limit(newLimit)` | 设置新 limit |
| `flip()` | 切换到读模式 |
| `clear()` | 重置为初始状态 |
| `mark()` | 标记当前位置 |
| `reset()` | 返回标记位置 |
| `rewind()` | 返回起始位置 |
| `compact()` | 压缩缓冲区 |
| `hasRemaining()` | 检查是否还有数据（const） |
| `remaining()` | 返回剩余字节数（const） |

### 其他方法

| 方法 | 描述 |
|------|------|
| `equals(other)` | 比较两个 ByteBuffer 是否相等 |
| `duplicate()` | 复制一个 ByteBuffer |
| `printInfo()` | 打印缓冲区信息 |

### Java 兼容接口

#### 数组访问方法

| 方法 | 描述 |
|------|------|
| `hasArray()` | 检查是否有可访问数组（始终返回 true） |
| `array()` | 返回底层字节数组指针 |
| `const array() const` | 返回底层字节数组指针（const 版本） |
| `arrayOffset()` | 返回数组偏移量（始终返回 0） |

#### 直接缓冲区方法

| 方法 | 描述 |
|------|------|
| `isDirect()` | 判断是否为直接缓冲区（当前实现返回 false） |

#### 字节序方法

| 方法 | 描述 |
|------|------|
| `ByteOrder` | 枚举类型：`ORDER_BIG_ENDIAN` / `ORDER_LITTLE_ENDIAN` |
| `order()` | 获取当前字节序 |
| `order(ByteOrder)` | 设置字节序，返回 `ByteBuffer&` 支持链式调用 |
| `static nativeOrder()` | 获取主机字节序 |

**字节序使用示例：**

```cpp
#include "ByteBuffer.h"

ByteBuffer bb;

// 设置为大端字节序（网络字节序）
bb.order(ByteOrder::ORDER_BIG_ENDIAN);

// 写入数据（自动按大端字节序存储）
bb.putInt(0x12345678);  // 缓冲区中字节顺序：12 34 56 78

// 切换为小端字节序
bb.order(ByteOrder::ORDER_LITTLE_ENDIAN);
bb.putInt(0x12345678);  // 缓冲区中字节顺序：78 56 34 12

// 读取时自动转换
bb.flip();
bb.order(ByteOrder::ORDER_BIG_ENDIAN);
uint32_t value = bb.getInt();  // 正确读取 0x12345678
```

#### 比较和辅助方法

| 方法 | 描述 |
|------|------|
| `compareTo(const ByteBuffer&)` | 按字典序比较两个缓冲区，返回负数/0/正数 |
| `hash()` | 计算缓冲区内容的哈希值 |
| `toString()` | 返回缓冲区的字符串表示 |

**使用示例：**

```cpp
// 比较缓冲区
ByteBuffer bb1, bb2;
// ... 填充数据 ...
int result = bb1.compareTo(bb2);  // <0: bb1<bb2, =0: 相等，>0: bb1>bb2

// 哈希值（可用于 unordered_map 等容器）
size_t h = bb1.hash();

// 字符串表示
std::string str = bb1.toString();  // "ByteBuffer[pos=5 lim=10 cap=100]"
```

## 构建与测试

### 依赖

- C++11 兼容编译器
- CMake 3.10+（仅用于构建测试）

### 使用方法（Header-Only）

由于是 Header-Only 库，只需将 `ByteBuffer.h` 复制到你的项目中即可使用：

```cpp
#include "ByteBuffer.h"

int main() {
    ByteBuffer bb;
    bb.putInt(12345);
    return 0;
}
```

**无需编译链接额外的库文件**，因为所有实现都在头文件中。

### 构建（仅用于测试）

如果你想运行测试：

```bash
mkdir build && cd build
cmake ..
make
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

1. **拷贝控制**：ByteBuffer 禁用了拷贝构造函数和拷贝赋值运算符，以防止浅拷贝导致的双重释放问题。如果需要拷贝，请使用 `duplicate()` 方法或移动语义。

2. **内存对齐**：对于多字节类型的读写操作，某些架构（如 ARM）可能需要内存对齐。未对齐的访问可能导致性能下降或崩溃。建议在处理大量数据时确保数据对齐。

3. **字节序**：本实现支持字节序设置，默认使用主机字节序。可通过 `order()` 方法设置大端或小端字节序，适用于跨平台数据序列化和网络传输。

4. **异常处理**：当内存分配失败时，会抛出 `std::bad_alloc` 异常。请在调用时确保有适当的异常处理。

## 参考链接

- [Java ByteBuffer 文档](https://docs.oracle.com/javase/7/docs/api/java/nio/ByteBuffer.html)
- [Netty ByteBuf](https://netty.io/4.0/api/io/netty/buffer/ByteBuf.html)
