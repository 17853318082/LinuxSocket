/*
   创建多个线程操作
   使用容器管理多个线程
*/

#include<iostream>
#include<thread>
#include<vector>
using namespace std;

void MyThread(int i){
    cout<<"线程编号"<<this_thread::get_id()<<"  "<<i<<endl;
}

int main(){
    // 1.创建和等待多个线程
    vector<thread> my_thread;
    for(int i=0;i<10;i++){
        my_thread.push_back(thread(MyThread,i+1));
    }
    for(int i=0;i<10;i++){
        my_thread[i].join();
    }
    cout<<"主线程结束"<<endl;
    return 0;
}