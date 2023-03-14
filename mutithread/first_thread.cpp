#include <thread>
#include <iostream>

using namespace std;

void ThreadMain(){
    cout<<"begin sub thread:"<<this_thread::get_id<<endl;
    for(int i=0;i<10;i++){
        cout<<"in thread:"<<i<<endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout<<"end sub thread:"<<this_thread::get_id<<endl;
}
void ThreadVal(int a,int b, int c){
    cout<<"结果为："<<a+b+c<<endl;
    return;
}

int main(){
    cout<<"main thread id:"<<this_thread::get_id<<endl;
    thread th1(ThreadMain);
    th1.detach();                       // 子线程与主线程分离   -- 守护线程
    //坑1： 主线程退出后，子线程不一定退出

    thread th(ThreadMain);       // 创建子进程并运行
    cout<<"begin wait sub thread"<<endl;  
    th.join();                       // 进程进入阻塞状态， 线程th结束后，父线程继续运行
    cout<<"end wait sub thread"<<endl;

    return 0;
}