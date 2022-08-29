# WebServer

用C++实现的高性能WEB服务器

## 功能

- 利用IO复用技术Epoll与线程池实现多线程的Reactor高并发模型；
- 利用正则与状态机解析HTTP请求报文，实现处理静态资源的请求；
- 利用vector容器封装char，并实现缓冲区的自动扩容；
- 基于堆结构实现的定时器，关闭超时的非活动连接；
- 改进了线程池的实现，优化了性能；

## 环境要求

- Linux
- C++11

## 项目启动

```
mkdir bin
make
./bin/Acgblog
```
