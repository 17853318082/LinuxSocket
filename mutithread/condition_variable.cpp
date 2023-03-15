/*
  模拟生产者消费者模型
*/
#include <thread>
#include <iostream>
#include <mutex>
#include <list>
#include <string>
#include <sstream>
#include <condition_variable> // 条件变量头文件
using namespace std;

list<string> msgs_;
mutex mux;             // 锁资源对象
condition_variable cv; // 条件变量

void ThreadWrite()
{
    for (int i = 0; i < 100; i++)
    {
        stringstream ss;
        ss << "第" << i << "次写入";
        unique_lock<mutex> lock(mux); // 获取锁资源     unique_lock锁管理器
        msgs_.push_back(ss.str());    // 写入数据
        lock.unlock();                // 写完出具进行解锁
        cv.notify_one();              // 通知其中一个消费者进程，进入消费
        // cv.notify_all();             // 线程锁定的资源都会释放
        this_thread::sleep_for(1s);   // 等待一秒，消费者线程获取锁资源
    }
}
void ThreadRead(int i)
{
    while (true)
    {

        cout << "read msg" << endl;
        unique_lock<mutex> lock(mux);
        // cv.wait(lock); // 解锁，阻塞等待信号
        // lamda 表达式
        cv.wait(lock,[i]{
            cout<<i<<"wait"<<endl;
            return !msgs_.empty();    // 当消息队列不为空的时候才执行，为空进入阻塞状态
        });
        // 获取信号后锁定
        while (!msgs_.empty())
        {
            cout <<i<<" read:" << msgs_.front() << endl; // 如果不是空的打印资源
            msgs_.pop_front();                        // 弹出元素
        }
    }
}

int main()
{
    thread th(ThreadWrite); // 创建一个生产者线程
    th.detach();
    // 创建三个消费者线程
    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadRead,i+1);
        th.detach();
    }
    getchar();
    return 0;
}