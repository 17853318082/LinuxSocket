/*
  模拟实现处理业务逻辑类声明
*/
#pragma once
#include "xthread.h"
#include "xthread.cpp"
#include <mutex>
#include <string>
#include <list>
#include <condition_variable>

class XMsgServer : public XThread
{
public:
  // 给当前线程发消息
  void SendMsg(std::string msg);
  // 判断进程退出
  void Stop();

private:
  // 处理消息的线程入口函数
  void Main() override;

  // 消息缓冲队列
  std::list<std::string> msgs_;

  // 互斥访问消息队列
  std::mutex mux_;

  // 条件变量
  std::condition_variable cv_;
};
