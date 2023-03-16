/*
多路IO转接--epoll
*/
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include "my_socket.cpp"
#include <string.h>
#include <sys/epoll.h>

using namespace std;
#define SERVER_PORT 8000
#define FD_SETSIZE __FD_SETSIZE

int main()
{
    int s_sock;
    int cfd;
    int read_len = 0;
    char buf[BUFSIZ]; // 读取数据缓冲区
    struct sockaddr_in server_addr, client_addr;
    int client[FD_SETSIZE]; // 定义一个数组用于记录用户字节符
    memset(client, -1, sizeof(int)); // 将数组初始化为-1
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

    // 创建一个监听红黑树
    int epfd = epoll_create(1024);
    // tep 设置单个fd的属性，ep时epoll_wait 传出的满足监听事件的数组
    struct epoll_event tep, ep[1024];
    tep.events = EPOLLIN; // 初始化s_sock的监听属性   写
    tep.data.fd = s_sock;
    // 将服务器字节符添加到监听红黑树上
    epoll_ctl(epfd, EPOLL_CTL_ADD, s_sock, &tep);
    int ret = 0; // 满足条件的字节符个数
    // 循环监听
    while (1)
    {
        ret = epoll_wait(epfd, ep, 1024, -1);
        for (int i = 0; i < ret; i++)
        {
            // s_sock满足读事件，有新的客户端发起链接请求
            if (ep[i].data.fd == s_sock)
            {
                cfd = Accept(s_sock,(struct sockaddr*)&client_addr,&client_addr_len);            // 获取客户端链接并加入到红黑树中
                tep.events = EPOLLIN; // 初始化cfd的监听属性
                tep.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &tep); // 将cfd加入到监听红黑树
            }
            else
            { // 如果不是s_sock, 集cfd满足读事件，有客户端写数据来
                read_len = Read(ep[i].data.fd, buf, sizeof(buf));
                // 客户端已关闭，无数据发过来，那么将该节点删除
                if (read_len == 0)
                {
                    close(ep[1].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep[i].data.fd, NULL); // 将关闭的cfd，从监听树上摘下
                    cout<<"客户端退出，关闭客户端字节符"<<endl;
                }
                else if (read_len > 0)
                {
                    // 读取缓冲区的内容
                    for (int i = 0; i < read_len; i++)
                    {
                        buf[i] = toupper(buf[i]);
                    }
                    // 回复客户端
                    Write(ep[i].data.fd, buf, read_len);
                    // 打印到控制台
                    Write(STDOUT_FILENO, buf, read_len);
                }
            }
        }
    }
    // 关闭服务器socket
    Close(s_sock);
}