# WebServer

Linux下C++轻量级Web服务器的主要工作：

- 使用多线程充分利用多核CPU，增加并行服务数量，并使用**线程池**避免线程频繁创建销毁的开销；
- 使用**非阻塞socket+epoll (LT和ET均实现)+事件处理(Reactor和Proactor均实现)** 的并发模型；
- 使用**状态机**解析 HTTP 请求报文，支持**GET/POST**请求，支持长/短连接；
- 采用基于升序双向链表的**定时器**，定时检测并断开非活跃用户；
- 实现**同步/异步日志系统**，记录服务器运行状态； 
- 访问服务器数据库实现 web 端用户**注册、登录**功能，可以请求服务器**图片和视频**文件;
- 经Webbench压力测试可以实现**上万的并发连接**数据交换 

# 

# 快速运行

- 服务器测试环境
  
  ```C++
      Ubuntu版本18.04
      MySQL版本5.7.38  
  ```

- 浏览器测试环境
  
  ```C++
      Linux
      dge
  ```

- 测试前已安装MySQL数据库
  
  ```C++
  // 建立yourdb库
  create database yourdb;
  // 创建user表
  USE yourdb;
  CREATE TABLE user(
      username char(50) NULL,
      passwd char(50) NULL
  )ENGINE=InnoDB;
  // 添加数据
  INSERT INTO user(username, passwd) VALUES('name', 'passwd');
  ```

- 修改main.cpp中的数据库初始化信息
  
  ```C++
  //数据库登录名,密码,库名
  string user = "root";
  string passwd = "root";
  string databasename = "yourdb";
  ```

- build
  
  ```C++
  sh ./build.sh
  ```

- 启动server
  
  ```C++
  ./server
  ```

- 浏览器端
  
  ```C++
  ip:9006
  ```
  
  # 个性化运行
  
  ```C++
  ./server [-p port] [-l LOGWrite] [-m TRIGMode] [-o OPT_LINGER] [-s sql_num] [-t thread_num] [-c close_log] [-a actor_model]
  ```

- -p，自定义端口号
  
  * 默认9006

- -l，选择日志写入方式，默认同步写入
  
  * 0，同步写入
  * 1，异步写入

- -m，listenfd和connfd的模式组合，默认使用LT + LT
  
  * 0，表示使用LT + LT
  * 1，表示使用LT + ET
  * 2，表示使用ET + LT
  * 3，表示使用ET + ET

- -o，优雅关闭连接，默认不使用
  
  * 0，不使用
  * 1，使用

- -s，数据库连接数量
  
  * 默认为8

- -t，线程数量
  
  * 默认为8

- -c，关闭日志，默认打开
  
  * 0，打开日志
  * 1，关闭日志

- -a，选择反应堆模型，默认Proactor
  
      * 0，Proactor模型
      * 1，Reactor模型

测试示例命令与含义

```C++
./server -p 9007 -l 1 -m 0 -o 1 -s 10 -t 10 -c 1 -a 1
```

- [x] 端口9007
- [x] 异步写入日志
- [x] 使用LT + LT组合
- [x] 使用优雅关闭连接
- [x] 数据库连接池内有10条连接
- [x] 线程池内有10条线程
- [x] 关闭日志
- [x] Reactor反应堆模型

# 压力测试

在关闭日志后，使用Webbench对服务器进行压力测试，对listenfd和connfd分别采用ET和LT模式，均可实现上万的并发连接，下面列出的是两者组合后的测试结果.

- Proactor，LT + LT，25023 QPS

- Proactor，LT + ET，16876 QPS

- Proactor，ET + LT，17943 QPS

- Proactor，ET + ET，17428 QPS

- Reactor，LT + ET，8624 QPS

- 并发连接总数：10000

- 访问服务器时间：5s

- 所有访问均成功
