#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>

#include <string>
#include <iostream>
#include <stdexcept>

// Default size of the buffer
constexpr uint32_t DEFAULT_BUFFER_SIZE = 2048;

// Mark value indicating no mark is set
constexpr uint32_t MARK_UNSET = UINT32_MAX;

class ByteBuffer
{
public:
    // Disable copy constructor and assignment operator to prevent double-free issues caused by shallow copy
    ByteBuffer(const ByteBuffer&) = delete;
    ByteBuffer& operator=(const ByteBuffer&) = delete;

    // Allow move semantics
    ByteBuffer(ByteBuffer&& other) noexcept
        : mark_(other.mark_),
          limit_(other.limit_),
          position_(other.position_),
          capacity_(other.capacity_),
          name_(std::move(other.name_)),
          buffer_(std::move(other.buffer_))
    {
        other.mark_ = MARK_UNSET;
        other.limit_ = 0;
        other.position_ = 0;
        other.capacity_ = 0;
    }

    ByteBuffer& operator=(ByteBuffer&& other) noexcept
    {
        if (this != &other)
        {
            mark_ = other.mark_;
            limit_ = other.limit_;
            position_ = other.position_;
            capacity_ = other.capacity_;
            name_ = std::move(other.name_);
            buffer_ = std::move(other.buffer_);
            other.mark_ = MARK_UNSET;
            other.limit_ = 0;
            other.position_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ByteBuffer(uint32_t capacity = DEFAULT_BUFFER_SIZE, const char* name = "")
        : mark_(MARK_UNSET),
          limit_(capacity),
          position_(0),
          capacity_(capacity),
          name_(name),
          buffer_(capacity, 0)
    {
    }

    ByteBuffer(uint8_t* arr, uint32_t length, const char* name = "")
        : mark_(MARK_UNSET),
          limit_(length),
          position_(0),
          capacity_(length),
          name_(name),
          buffer_(length, 0)
    {
        putBytes(arr, capacity_);
        clear();
    }

    ~ByteBuffer() = default;

    // Write Methods
    ByteBuffer& put(ByteBuffer* bb)
    {
        for (uint32_t i = 0; i < bb->limit(); i++)
            append<uint8_t>(bb->get(i));

        return *this;
    }
    ByteBuffer& put(uint8_t value)
    {
        append<uint8_t>(value);
        return *this;
    }
    ByteBuffer& put(uint8_t value, uint32_t index)
    {
        insert<uint8_t>(value, index);
        return *this;
    }
    ByteBuffer& putBytes(const uint8_t* buf, uint32_t len)
    {
        for (uint32_t i = 0; i < len; i++)
            append<uint8_t>(buf[i]);

        return *this;
    }
    ByteBuffer& putBytes(const uint8_t* buf, uint32_t len, uint32_t index)
    {
        position_ = index;
        for (uint32_t i = 0; i < len; i++)
            append<uint8_t>(buf[i]);

        return *this;
    }
    ByteBuffer& putChar(char value)
    {
        append<uint8_t>(value);
        return *this;
    }
    ByteBuffer& putChar(char value, uint32_t index)
    {
        insert<uint8_t>(value, index);
        return *this;
    }
    ByteBuffer& putShort(uint16_t value)
    {
        append<uint16_t>(value);
        return *this;
    }
    ByteBuffer& putShort(uint16_t value, uint32_t index)
    {
        insert<uint16_t>(value, index);
        return *this;
    }
    ByteBuffer& putInt(uint32_t value)
    {
        append<uint32_t>(value);
        return *this;
    }
    ByteBuffer& putInt(uint32_t value, uint32_t index)
    {
        insert<uint32_t>(value, index);
        return *this;
    }
    ByteBuffer& putLong(uint64_t value)
    {
        append<uint64_t>(value);
        return *this;
    }
    ByteBuffer& putLong(uint64_t value, uint32_t index)
    {
        insert<uint64_t>(value, index);
        return *this;
    }
    ByteBuffer& putFloat(float value)
    {
        append<float>(value);
        return *this;
    }
    ByteBuffer& putFloat(float value, uint32_t index)
    {
        insert<float>(value, index);
        return *this;
    }
    ByteBuffer& putDouble(double value)
    {
        append<double>(value);
        return *this;
    }
    ByteBuffer& putDouble(double value, uint32_t index)
    {
        insert<double>(value, index);
        return *this;
    }

    // Read Methods
    uint8_t get()
    {
        return read<uint8_t>();
    }
    uint8_t get(uint32_t index) const
    {
        return read<uint8_t>(index);
    }
    void getBytes(uint8_t* buf, uint32_t len)
    {
        if (buffer_.empty() || position_ + len > limit_)
            return;

        memcpy(buf, buffer_.data() + position_, len);
        position_ += len;
    }
    void getBytes(uint32_t index, uint8_t* buf, uint32_t len) const
    {
        if (buffer_.empty() || index + len > limit_)
            return;

        memcpy(buf, buffer_.data() + index, len);
    }
    char getChar()
    {
        return read<uint8_t>();
    }
    char getChar(uint32_t index) const
    {
        return read<uint8_t>(index);
    }
    uint16_t getShort()
    {
        return read<uint16_t>();
    }
    uint16_t getShort(uint32_t index) const
    {
        return read<uint16_t>(index);
    }
    uint32_t getInt()
    {
        return read<uint32_t>();
    }
    uint32_t getInt(uint32_t index) const
    {
        return read<uint32_t>(index);
    }
    uint64_t getLong()
    {
        return read<uint64_t>();
    }
    uint64_t getLong(uint32_t index) const
    {
        return read<uint64_t>(index);
    }
    float getFloat()
    {
        return read<float>();
    }
    float getFloat(uint32_t index) const
    {
        return read<float>(index);
    }
    double getDouble()
    {
        return read<double>();
    }
    double getDouble(uint32_t index) const
    {
        return read<double>(index);
    }

    bool equals(const ByteBuffer* other) const
    {
        uint32_t len = limit();
        if (len != other->limit())
            return false;

        for (uint32_t i = 0; i < len; i++)
        {
            if (get(i) != other->get(i))
                return false;
        }

        return true;
    }
    ByteBuffer* duplicate()
    {
        ByteBuffer* newBuffer = new ByteBuffer(capacity_);

        // copy data
        newBuffer->put(this);

        newBuffer->limit(limit_);
        newBuffer->position(position_);

        return newBuffer;
    }

    ByteBuffer& clear()
    {
        position_ = 0;
        mark_     = MARK_UNSET;
        limit_    = capacity_;
        return *this;
    }
    ByteBuffer& flip()
    {
        limit_    = position_;
        position_ = 0;
        mark_     = MARK_UNSET;
        return *this;
    }
    ByteBuffer& mark()
    {
        mark_ = position_;
        return *this;
    }

    ByteBuffer& discardMark()
    {
        mark_ = MARK_UNSET;
        return *this;
    }
    ByteBuffer& reset()
    {
        if (mark_ != MARK_UNSET)
            position_ = mark_;

        return *this;
    }

    ByteBuffer& rewind()
    {
        mark_ = MARK_UNSET;
        position_ = 0;

        return *this;
    }

    ByteBuffer& compact()
    {
        if (position_ >= limit_)
        {
            position_ = 0;
        }
        else
        {
            uint32_t remaining = limit_ - position_;
            memmove(buffer_.data(), &buffer_[position_], remaining);
            position_ = remaining;
        }

        limit_ = capacity_;
        return *this;
    }


    bool hasRemaining() const
    {
        return limit_ > position_;
    }
    uint32_t remaining() const
    {
        return position_ < limit_ ? limit_ - position_ : 0;
    }
    uint32_t capacity() const
    {
        return capacity_;
    }
    uint32_t position() const
    {
        return position_;
    }
    uint32_t limit() const
    {
        return limit_;
    }

    ByteBuffer& limit(uint32_t newLimit)
    {
        if (position_ > newLimit)
            position_ = newLimit;

        if (mark_ > newLimit)
            mark_ = MARK_UNSET;

        return *this;
    }

    ByteBuffer& position(uint32_t newPosition)
    {
        position_ = newPosition;
        return *this;
    }

    void printInfo() const
    {
        std::cout << "ByteBuffer " << name_ << ":\n"
                  << "\tmark(" << mark_ << "), "
                  << "position(" << position_ << "), "
                  << "limit(" << limit_ << "), "
                  << "capacity(" << capacity_ << ")." << std::endl;
    }

private:
    template <typename T>
    T read(uint32_t index) const
    {
        if (index + sizeof(T) > limit_)
            return 0;

        T data;
        memcpy(&data, &buffer_[index], sizeof(T));
        return data;
    }

    template <typename T>
    T read()
    {
        T data = read<T>(position_);
        position_ += sizeof(T);
        return data;
    }


    template<typename T>
    void append(T data)
    {
        uint32_t s = sizeof(data);
        checkSize(s);

        memcpy(&buffer_[position_], reinterpret_cast<uint8_t*>(&data), s);
        position_ += s;
    }

    template<typename T>
    void insert(T data, uint32_t index)
    {
        uint32_t s = sizeof(data);
        checkSize(index, s);

        position_ = index;
        append<T>(data);
    }


    void checkSize(uint32_t increase)
    {
        checkSize(position_, increase);
    }

    void checkSize(uint32_t index, uint32_t increase)
    {
        if (index + increase <= capacity_)
            return;

        uint32_t newSize = capacity_ + (increase + BUFFER_SIZE_INCREASE - 1) /
                                           BUFFER_SIZE_INCREASE * BUFFER_SIZE_INCREASE;
        buffer_.resize(newSize, 0);
        capacity_ = newSize;
    }

private:
    const uint32_t   BUFFER_SIZE_INCREASE = 2048;
    std::string      name_;
    std::vector<uint8_t> buffer_;
    uint32_t         mark_;
    uint32_t         limit_;
    uint32_t         position_;
    uint32_t         capacity_;

};

#endif
