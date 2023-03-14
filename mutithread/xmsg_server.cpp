
#include "xmsg_server.h"

using namespace std;
using namespace this_thread;

// 给当前线程发送消息
void XMsgServer::SendMsg(std::string msg)
{
    unique_lock<mutex> loc(mux_);
    msgs_.push_back(msg);
}
// 处理消息队列
void XMsgServer::Main()
{
    while (!is_exit())
    {
        sleep_for(10ms);
        unique_lock<mutex> loc(mux_);
        // 如果消息为空
        if (msgs_.empty())
        {
            continue;
        }
        // 处理消息队列
        while (!msgs_.empty())
        {
            cout << "recv:" << msgs_.front() << endl;
            msgs_.pop_front();
        }
    }
}