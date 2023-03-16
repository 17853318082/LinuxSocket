/*
简易的多线程服务器
开发人：王世玉
时间：2023/3/16
*/

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include "my_socket.cpp"

using namespace std;

#define SERVER_PORT 8000 // 端口号宏

void ThreadMain(int c_sock)
{
    // 子线程开始
    cout << "子线程:" << this_thread::get_id() << " 开启服务" << endl;
    // 设置读取缓冲区
    char buf[BUFSIZ];
    // 循环监听客户端请求
    bool run = true;
    while (run)
    {
        // 读取字节长度
        int read_len = Read(c_sock, buf, sizeof(buf));
        // 未读取到数据，退出
        if (read_len == 0)
        {
            // 关闭客户端
            cout << "客户端无消息传回，关闭客户端socket" << endl;
            Close(c_sock);
            return;
        }
        // 客户端已发送数据，根据接受数据回应客户端
        // 小写转大写
        for (int i = 0; i < read_len; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        // 回复客户端
        Write(c_sock, buf, read_len);
        // 打印到控制台
        Write(STDOUT_FILENO, buf, read_len);
    }
    // 子线程结束,关闭客户端socket
    cout << "客户端请求结束，关闭客户端" << endl;
    Close(c_sock);
    cout << "子线程:" << this_thread::get_id() << " 退出服务" << endl;
    return;
}

int main()
{
    int s_sock;
    int c_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 获取socket字节符
    s_sock = Socket(AF_INET, SOCK_STREAM, 0);
    // 设置端口复用
    int opt = 1;
    setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));

    // 用bind绑定ip
    Bind(s_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // listen,最大监听数
    Listen(s_sock, 256);
    // 进入循环监听，使用多线程
    bool run = true;
    while (run)
    {
        // accept监听用户新消息
        c_sock = Accept(s_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (c_sock == -1)
        {
            // 无客户端访问，跳过线程开启
            continue;
        }
        else
        {
            // 创建一个线程
            cout << "创建一个子线程" << endl;
            thread sub_thread(ThreadMain, c_sock);
            // 将定义线程与主线程链接
            // if (sub_thread.joinable())
            // {
            //     sub_thread.join();
            // }
            sub_thread.detach();
        }
    }
    // 关闭服务器socket
    Close(s_sock);
}