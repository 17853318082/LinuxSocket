/*
  共享数据可能遇到的问题
  1. 只读数据时安全稳定的，不需要特别的处理
  2. 有读有写，

  c++解决对线程数据共享保护问题
*/

#include <iostream>
#include <thread>
#include <list>
#include <mutex>

using namespace std;

mutex mux;

class A
{
public:
    // 把收到的消息，加入到一个队列的线程
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 1000000; i++)
        {
            mux.lock();
            cout << "写入一个元素" << i << endl;
            msgRecvQueue.push_back(i);
            mux.unlock();
            this_thread::sleep_for(50ms);
        }
    }
    // 把数据从消息队列中取出
    void outMsgRecvQueue()
    {
        for (int i = 0; i < 1000000; i++)
        {
            if (!msgRecvQueue.empty())
            {
                mux.lock();
                int command = msgRecvQueue.front();
                cout << "读出一个命令" << command << endl;
                msgRecvQueue.pop_front();
                mux.unlock();
                this_thread::sleep_for(50ms);
            }
            else
            {
                cout << "消息队列为空！" << endl;
                this_thread::sleep_for(50ms);
            }
        }
    }

private:
    list<int> msgRecvQueue; // 消息队列，共享数据
};

int main()
{
    // 共享数据的保护案例
    /*
    网络游戏服务器，两个自己创建的线程，一个线程手机玩家的命令，并把命令数据写到一个队列中
    另一个线程，从队列中去除玩家发送来的命令，解析，然后执行玩家需要的动作
    */
    // 用成员函数作为线程函数的方法

    // 创建一个别进程
    A myObject;
    thread myInMsg(&A::inMsgRecvQueue, &myObject);   // 创建谢进程
    thread myOutMsg(&A::outMsgRecvQueue, &myObject); // 创建读进程
    myInMsg.join();
    myOutMsg.join();

    return 0;
}