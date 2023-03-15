#include <iostream>
#include <thread>

using namespace std;

void ThreadMain()
{
    cout << "我是一个子线程" << endl;
    cout << "当前子线程id：" << this_thread::get_id() << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << "子线程：" << i << endl;
        this_thread::sleep_for(100ms);
    }
    cout << "子线程执行结束" << endl;
}

int main()
{
    cout << "主线程id：" << this_thread::get_id() << endl;   // this_thread::get_id()  获取当前线程id
    thread my_thread(ThreadMain);
    cout << "是否可join" << my_thread.joinable() << endl; // joinable是否可join
    // my_thread.join();                  // 线程阻塞，主线程等待子线程执行完毕
    my_thread.detach();                                   // 子线程与主线程分离，detach后就不能再次join
    cout << "是否可join" << my_thread.joinable() << endl; // joinable是否可join
    for (int i = 0; i < 5; i++)
    {
        cout << "主线程" << i << endl;
        this_thread::sleep_for(1s);
    }
    cout << my_thread.joinable() << endl;
    cout << "是否可join"
         << "主线程结束" << endl;

    return 0;
}