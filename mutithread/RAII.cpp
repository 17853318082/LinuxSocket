/*
  RAII管理锁资源：将锁做成一个类
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>

using namespace std;

//RAII
class XMutex
{
public:
    XMutex(mutex &mux) : mux_(mux)
    {
        cout << "Lock" << endl;
    }
    ~XMutex()
    {
        cout << "unLock" << endl;
    }

private:
    mutex &mux_;
};
// 测试锁资源自动获取释放
static mutex mux;
void TestMutex(int status){
    XMutex lock(mux);
    if(status ==1){
        cout<<1<<endl;
        return;
    }else{
        cout<<status<<endl;
        return;
    }

}
int main(){
    TestMutex(1);
    TestMutex(2);
}