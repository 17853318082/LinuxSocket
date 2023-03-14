/*
  封装线程类
  谷歌命名格式，文件，类，函数命名为大驼峰
  私有成员变量用小写加下划线
  私有成员对外开放的成员函数，与私有成员同名只是在名字最后少一个下划线
  头文件内，仅定义
*/

#include <iostream>
#include <string>
#include <thread>

#ifndef _WRAP_H_
#define _WRAP_H_

class XThread
{
public:
    virtual void Start(); // 进程开始入口
    virtual void Stop();  // 线程结束
    virtual void Wait();  // 线程加入
    bool is_exit();

private:
    virtual void Main() = 0;
    std::thread th_;       // 声明一个线程对象
    bool is_exit_ = false; // 退出标记
};

#endif