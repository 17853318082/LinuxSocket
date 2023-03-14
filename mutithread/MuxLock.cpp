/*
  问题：有的时候会出现，线程占用资源不释放，其他线程无法获得资源
*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>

void ThreadMainMux(int val){
    std::mutex mux;            // 锁对象
    while(true){
        mux.lock();              // 线程获取资源
        std::cout<<val<<std::endl;
        sleep(1);               // 暂停一秒
        mux.unlock();
    }
}
int main(){
    for(int i=0;i<3;i++){
        std::thread th(ThreadMainMux,i);  // 创建线程对象
        th.detach();              // 分离线程，获取让三个线程竞争资源
    }
}