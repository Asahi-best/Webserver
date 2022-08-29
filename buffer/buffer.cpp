#include "buffer.h"

Buffer::Buffer(int initBuffersize):buffer_(initBuffersize),  readPos_(0), writePos_(0){}

size_t Buffer::ReadableBytes() const
{
    return writePos_ - readPos_;
}

size_t Buffer::WriteableBytes() const
{
    return buffer_.size() - writePos_;
}

size_t Buffer::ReadBytes() const
{
    return readPos_;
}

const char* Buffer::ReadPtr() const
{
    return BeginPtr_() + readPos_;
}

void Buffer::UpdateReadPtr(size_t len)
{
    assert(len<=ReadableBytes());
    readPos_ += len;
}

void Buffer::UpdateReadPtrUntil(const char* end)
{
    assert(end>=ReadPtr());
    UpdateReadPtr(end - ReadPtr());
}

const char* Buffer::WritePtrConst() const
{
    return BeginPtr_() + writePos_;
}

char* Buffer::WritePtr()
{
    return BeginPtr_() + writePos_;
}

void Buffer::UpdateWritePtr(size_t len)
{
    assert(len<=WriteableBytes());
    writePos_ += len;
}

void Buffer::InitPtr()
{
    bzero(&buffer_[0], buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}

void Buffer::MakeSpace(size_t len)
{
    //如果剩余空间够就调整，否则申请空间
    if(WriteableBytes() + ReadBytes() < len) buffer_.resize(writePos_ + len + 1);
    else
    {
        //将读指针置零
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
        readPos_ = 0;
        writePos_ = readable;
        assert(readable == ReadableBytes());
    }
}

void Buffer::EnsureWriteable(size_t len)
{
    if(WriteableBytes() < len) MakeSpace(len);
    assert(WriteableBytes() >= len);
}

void Buffer::append(const char* str, size_t len)
{
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, WritePtr());
    UpdateWritePtr(len);
}

void Buffer::append(const std::string& str)
{
    append(str.data(), str.length());
}

void Buffer::append(const void* data, size_t len)
{
    assert(data);
    append(static_cast<const char*>(data), len);
}

void Buffer::append(const Buffer& buffer)
{
    append(buffer.ReadPtr(), buffer.ReadableBytes());
}

ssize_t Buffer::ReadFd(int fd, int* Errno)
{
    char buff[65535];   //暂时的缓冲区
    struct  iovec iov[2];
    const size_t writeable = WriteableBytes();

    iov[0].iov_base = BeginPtr_() + writePos_;
    iov[0].iov_len = writeable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);
    if(len < 0) *Errno = errno;
    else if(static_cast<size_t>(len) <= writeable) writePos_ += len;
    else
    {
        writePos_ = buffer_.size();
        append(buff, len - writeable);
    }
    return len;
}

ssize_t Buffer::WriteFd(int fd, int* Errno)
{
    size_t readsize = ReadableBytes();
    ssize_t len = write(fd, ReadPtr(), readsize);
    if(len < 0)
    {
        *Errno = errno;
        return len;
    }
    readPos_ += len;
    return len;
}

std::string Buffer::AlltoStr()
{
    std::string str(ReadPtr(), ReadableBytes());
    InitPtr();
    return str;
}

char* Buffer::BeginPtr_()
{
    return &*buffer_.begin();
}

const char* Buffer::BeginPtr_() const
{
    return &*buffer_.begin();
}