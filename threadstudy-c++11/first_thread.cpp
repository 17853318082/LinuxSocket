#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

struct test{
    string name;
    int age;
};
void *callback(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        cout << "子线程：" << i << endl;
    }
    cout << "子线程结束" << endl;
    struct test t;
    t.name = "1";
    t.age = 2;
    pthread_exit(&t);
    return NULL;
}
int main()
{
    pthread_t tid;                              // 声明线程对象
    pthread_create(&tid, NULL, callback, NULL); // 创建一个线程
    for (int i = 0; i < 10; i++)
    {
        cout << "主线程：" << i << endl;
    }
    void *ptr;
    pthread_join(tid,&ptr);                  // 回收tid进程 ，使用ptr接受子线程传出来的值
    struct test* pt = (struct test*)ptr;
    cout<<pt->name<<endl;
    cout<<pt->age<<endl;
    cout << "主线程结束" << endl;
    // pthread_exit(NULL); // 主线程退出，--调用线程退出函数，不影响子线程的运行
    return 0;
}