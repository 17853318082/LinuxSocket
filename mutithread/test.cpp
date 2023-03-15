#include "xmsg_server.cpp"
#include <sstream>

// using namespace std;

int main()
{
    XMsgServer xms;
    xms.Start();
    for (int i = 0; i < 10; i++)
    {
        stringstream ss;       // 创建一个string流对象
        ss << "msg:" << i + 1; // 将字符串写入流对象
        cout<<"写入："<<ss.str()<<endl;
        xms.SendMsg(ss.str());
        this_thread::sleep_for(500ms);
    }
    xms.Stop();
    getchar();
    return 0;
}