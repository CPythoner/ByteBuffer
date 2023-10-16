#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <string>
#include <iostream>

// Default size of the buffer
#define DEFAULT_BUFFER_SIZE 2048
#define DEFAULT_MARK_POSITION ((uint32_t)-1)

class ByteBuffer
{
public:
    ByteBuffer(uint32_t capacity = DEFAULT_BUFFER_SIZE, const char* name = "")
        : name_(name),
        mark_(DEFAULT_MARK_POSITION),
        limit_(capacity),
        position_(0),
        capacity_(capacity)
    {
        p_buffer_ = nullptr;
        p_buffer_ = static_cast<uint8_t*>(calloc(capacity_, sizeof(uint8_t)));
    }

    ByteBuffer(uint8_t* arr, uint32_t length, const char* name = "")
        : name_(name),
        mark_(DEFAULT_MARK_POSITION),
        limit_(length),
        position_(0),
        capacity_(length)
    {
        p_buffer_ = nullptr;
        p_buffer_ = static_cast<uint8_t*>(calloc(capacity_, sizeof(uint8_t)));

        putBytes(arr, length);
        flip();
    }
    ~ByteBuffer()
    {
        if (p_buffer_)
        {
            free(p_buffer_);
            p_buffer_ = nullptr;
        }
    }

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
        append(buf, len);
        return *this;
    }
    ByteBuffer& putBytes(const uint8_t* buf, uint32_t len, uint32_t index)
    {
        position_ = index;
        append(buf, len);
        return *this;
    }
    ByteBuffer& putString(const std::string& buf)
    {
        append(reinterpret_cast<const uint8_t*>(buf.data()), static_cast<uint32_t>(buf.size()));
        return *this;
    }
    ByteBuffer& putString(const std::string& buf, uint32_t len)
    {
        append(reinterpret_cast<const uint8_t*>(buf.data()), len);
        return *this;
    }
    ByteBuffer& putString(const std::string& buf, uint32_t len, uint32_t index)
    {
        position_ = index;
        append(reinterpret_cast<const uint8_t*>(buf.data()), len);
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
        if (0 == memcpy_s(buf, len, p_buffer_ + position_, remaining()))
            position_ = position_ + len;
    }
    void getBytes(uint32_t index, uint8_t* buf, uint32_t len) const
    {
        memcpy_s(buf, len, p_buffer_ + index, limit_ - index);
    }
    std::string getString(uint32_t len)
    {
        std::string result((char*)p_buffer_ + position_, len);
        position_ += len;
        return result;
    }
    std::string getString(uint32_t index, uint32_t len) const
    {
        return {p_buffer_ + index, p_buffer_ + index + len};
    }
    char getChar()
    {
        return read<char>();
    }
    char getChar(uint32_t index) const
    {
        return read<char>(index);
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

    bool equals(ByteBuffer* other)
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
        mark_     = DEFAULT_MARK_POSITION;
        limit_    = capacity_;
        return *this;
    }
    ByteBuffer& flip()
    {
        limit_    = position_;
        position_ = 0;
        mark_     = DEFAULT_MARK_POSITION;
        return *this;
    }
    ByteBuffer& mark()
    {
        mark_ = position_;
        return *this;
    }

    ByteBuffer& discardMark()
    {
        mark_ = DEFAULT_MARK_POSITION;
        return *this;
    }
    ByteBuffer& rewind()
    {
        if (mark_ != DEFAULT_MARK_POSITION)
            position_ = mark_;

        return *this;
    }

    ByteBuffer& reset()
    {
        mark_     = DEFAULT_MARK_POSITION;
        position_ = 0;
        return *this;
    }

    ByteBuffer& compact()
    {
        do
        {
            if (position_ >= limit_)
            {
                position_ = 0;
                break;
            }

            for (uint32_t i = 0; i < limit_ - position_; i++)
            {
                p_buffer_[i] = p_buffer_[position_ + i];
            }
            position_ = limit_ - position_;
        } while (false);

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
    uint32_t move(const uint32_t newPos)
    {
        if (newPos <= limit_)
            position_ = newPos;
        return position_;
    }
    uint32_t step(const int32_t steps)
    {
        int64_t newPos = position_ + steps;
        if (newPos >= 0 && newPos <= limit_)
            position_ = static_cast<uint32_t>(newPos);
        return position_;
    }
    uint32_t limit() const
    {
        return limit_;
    }

    ByteBuffer& limit(const uint32_t newLimit)
    {
        if (position_ > newLimit)
            position_ = newLimit;

        if (mark_ > newLimit)
            mark_ = DEFAULT_MARK_POSITION;

        return *this;
    }

    ByteBuffer& position(const uint32_t newPosition)
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
        if (!p_buffer_ || index + sizeof(T) > limit_)
            return 0;

        return *reinterpret_cast<T*>(&p_buffer_[index]);
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
        if (!p_buffer_)
            return;

        uint32_t s = sizeof(data);
        checkSize(s);

        memcpy(&p_buffer_[position_], (uint8_t*)&data, s);
        position_ += s;
    }
    void append(const uint8_t* data, uint32_t len)
    {
        if (!p_buffer_)
            return;

        checkSize(len);

        memcpy(&p_buffer_[position_], data, len);
        position_ += len;
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
        uint8_t* pBuf = (uint8_t*)realloc(p_buffer_, newSize);
        if (!pBuf)
        {
            std::cout << "relloc failed!" << std::endl;
            exit(1);
        }

        p_buffer_ = pBuf;
        capacity_ = newSize;
    }

private:
    const uint32_t   BUFFER_SIZE_INCREASE = 2048;
    std::string      name_;
    uint8_t*         p_buffer_;
    uint32_t         mark_;
    uint32_t         limit_;
    uint32_t         position_;
    uint32_t         capacity_;

};

#endif
