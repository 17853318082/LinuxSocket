
#include "xmsg_server.h"

using namespace std;
using namespace this_thread;

// 给当前线程发送消息
void XMsgServer::SendMsg(std::string msg)
{
    unique_lock<mutex> loc(mux_);
    msgs_.push_back(msg);
}
void XMsgServer::Stop()
{
    is_exit_ = true;
    Wait();
    cout << "线程退出" << endl;
}
// 处理消息队列
void XMsgServer::Main()
{
    while (!is_exit())
    {
        sleep_for(1000ms);
        unique_lock<mutex> lock(mux_);
        // 要向表达式中传入this，不然找不到类的方法
        cv_.wait(lock, [this]
                 {
                     if (is_exit())
                     {
                         return true;
                     }
                     cout << "wait cv" << endl;
                     return !msgs_.empty(); // 当想队列不为空时跳出阻塞进入执行
                 });
        // 处理消息队列
        while (!msgs_.empty())
        {
            cout << "recv:" << msgs_.front() << endl;
            msgs_.pop_front();
        }
        // this_thread::sleep_for(50ms);
    }
}