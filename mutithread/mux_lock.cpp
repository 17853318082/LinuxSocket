/*
  问题：有的时候会出现，线程占用资源不释放，其他线程无法获得资源
  lock() :获取锁资源
  try_lock_for(): 尝试获取锁资源
*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex> // 获取锁资源头文件
#include <unistd.h>
#include <shared_mutex> // 共享锁头文件   c++17

using namespace std;

// 获取锁资源 lock()
void ThreadMainMux(int val)
{
    std::mutex mux; // 锁对象
    while (true)
    {
        mux.lock(); // 线程获取资源
        std::cout << val << std::endl;
        sleep(1); // 暂停一秒
        mux.unlock();
    }
}
// 尝试获取锁资源
void ThreadMainTime(int i)
{
    timed_mutex tmux;
    while (true)
    {
        if (tmux.try_lock_for(chrono::microseconds(500)))
        { // 尝试获取锁资源，每0.5秒尝试一次
            cout << i << "[try_lock_for timeout]" << endl;
            continue;
        }
        cout << i << "[in]" << endl;
        this_thread::sleep_for(2000ms);
        tmux.unlock(); // 释放锁资源
        this_thread::sleep_for(1ms);
    }
}
// 可重入锁资源, 可以锁两次,也必须解锁两次
recursive_mutex rmux;
void Task1()
{
    rmux.lock();
    cout << "task1[in]" << endl;
    rmux.unlock();
}
void Task2()
{
    rmux.lock();
    cout << "task2[in]" << endl;
    rmux.unlock();
}
void ThreadMainReMux(int i)
{
    while (true)
    {
        rmux.lock();
        Task1();
        cout << i << "[in]" << endl;
        sleep(1);
        Task2();
        rmux.unlock();
        this_thread::sleep_for(1ms);
    }
}

// 共享锁
shared_timed_mutex stmux;
void ThreadRead(int i)
{
    // 读取锁
    while (true)
    {
        stmux.lock_shared(); // 获取共享锁，在读数据时，允许其他线程访问
        cout << i << "read" << endl;
        this_thread::sleep_for(500ms);
        stmux.unlock_shared(); // 释放共享锁
        this_thread::sleep_for(1ms);
    }
}
void ThreadWrite(int i)
{
    while (true)

    {
        stmux.lock_shared();
        // cout << "读取数据" << endl;
        stmux.unlock_shared();
        stmux.lock(); // 互斥锁写入  ,获取互斥锁后，其他线程无法访问
        cout << i << "Write" << endl;
        this_thread::sleep_for(500ms);
        stmux.unlock();
        this_thread::sleep_for(1ms);
    }
}



int main()
{
    // 测试获取锁资源 lock();
    // for (int i = 0; i < 3; i++)
    // {
    //     std::thread th(ThreadMainMux, i); // 创建线程对象
    //     th.detach();                      // 分离线程，获取让三个线程竞争资源
    // }

    // 测试 尝试获取锁资源 try_lock_for();
    // for(int i=0;i<3;i++){
    //     thread th(ThreadMainTime,i+1);
    //     th.detach();                      // 分离线程
    // }
    // getchar();                            // 按键盘结束

    // 重锁
    // for (int i = 0; i < 3; i++)
    // {
    //     thread th(ThreadMainReMux, i + 1);
    //     th.detach();
    // }
    // getchar();

    // 共享锁测试 
    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadRead, i + 1);
        th.detach();
    }
    for (int i = 0; i < 3; i++)
    {
        thread th(ThreadWrite, i + 1);
        th.detach();
    }
    getchar();

    return 0;
}