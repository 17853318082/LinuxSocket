#include <thread>
#include <iostream>
#include <string>
#include <unistd.h>
#include "xthread.cpp"

class MyThread
{
public:
    // 入口函数
    void Main()
    {
        std::cout << "成员函数" << name << ":" << age << std::endl;
    }
    std::string name;
    int age;
};
// 测试封装线程类
class TestXThread : public XThread
{ 
public:
    void Main() override
    { // override 检测重写函数是否正确
        std::cout << "TestXThread begin " << name << std::endl;
        bool run = true;
        int i = 0;
        while (run)
        {
            std::cout << ".";
            sleep(1); // 秒级睡眠
            // usleep(10000);             // 微秒级睡眠
            if (i > 10)
            {
                break;
            }
            i++;
        }
        std::cout << std::endl;
        std::cout << "TestXtread end" << std::endl;
    }
    std::string name;
};

// 测试线程类
int main()
{
    TestXThread txthread;
    txthread.name = "子线程01";
    txthread.Start();
    txthread.Stop();
    txthread.Wait();

    getchar();
    MyThread mythread; // 创建类对象
    mythread.name = "test name 01";
    mythread.age = 10;
    // 用成员函数作为子线程的入口函数，需要成员函数的地址(子线程接受函数时，实际上是接收函数的指针),
    // 并且需要传入对象的地址
    std::thread th(&MyThread::Main, &mythread);
    // th.join();               // 将线程th阻塞
    std::cout << "线程th已退出" << std::endl;
    return 0;
}