/*
  实现类中定义的函数
*/

#include "XThread.h"

void XThread::Start()
{
    std::cout << "创建子进程" << std::endl;
    is_exit_ = false;
    th_ = std::thread(&XThread::Main, this); // 创建一个线程对象,并执行Main方法
}
void XThread::Stop()
{
    is_exit_ = true; // 标记退出线程，等待线程退出
    Wait();
}
void XThread::Wait()
{
    if (th_.joinable())
    { // 是否可以进行链接，被销毁的线程无法加入
        th_.join();
    }
}
bool XThread::is_exit()
{ // 查看线程是否已退出
    return is_exit_;
}