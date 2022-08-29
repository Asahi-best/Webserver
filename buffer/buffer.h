#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <atomic>
#include<unistd.h> //read() write()
#include<sys/uio.h> //readv() writev()
#include <assert.h>

class Buffer
{
public:
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;

    size_t ReadableBytes() const;   //缓冲区可读字节数
    size_t WriteableBytes() const;  //缓冲区可写字节数
    size_t ReadBytes() const;   //缓冲区已读字节数

    const char* ReadPtr() const;    //获取读指针
    void UpdateReadPtr(size_t len); //更新读指针
    void UpdateReadPtrUntil(const char* end);   //更新读指针到指定位置
    
    const char* WritePtrConst() const;
    char* WritePtr();
    void UpdateWritePtr(size_t len);

    void InitPtr(); //初始化指针

    void EnsureWriteable(size_t len);   //保证将数据写入缓冲区
    void append(const char* str, size_t len);
    void append(const std::string& str);
    void append(const void* data, size_t len);
    void append(const Buffer& buffer);

    //IO操作读写接口
    ssize_t ReadFd(int fd, int* Errno);
    ssize_t WriteFd(int fd, int* Errno);

    //将缓冲区的数据转化为字符串
    std::string AlltoStr();

private:
    //返回指向缓冲区初始位置的指针
    char* BeginPtr_();
    const char* BeginPtr_() const;

    void MakeSpace(size_t len); //扩容

    std::vector<char>buffer_;   //buffer实体
    std::atomic<std::size_t>readPos_;
    std::atomic<std::size_t>writePos_;
};

#endif  //BUFFER_H