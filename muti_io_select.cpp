/*
简易的IO多路复用服务器--select实现
开发人：王世玉
时间：2023/3/16
*/

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include "my_socket.cpp"
#include <string.h>

using namespace std;
#define SERVER_PORT 8000
#define FD_SETSIZE __FD_SETSIZE

int main()
{
    int s_sock;
    int c_sock;
    int maxfd = 0;
    int nready;
    char buf[BUFSIZ]; // 读取数据缓冲区
    struct sockaddr_in server_addr, client_addr;
    int client[FD_SETSIZE]; // 定义一个数组用于记录用户字节符
    int maxp = 0;
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

    // rset读事件文件描述符集合  allset用来备份
    fd_set rset, allset;
    // 最大文件描述符
    maxfd = s_sock;
    // 清空监听集合
    FD_ZERO(&allset);
    // 构造select监控文件描述符集
    FD_SET(s_sock, &allset);

    // 循环监听
    while (1)
    {
        // 备份
        rset = allset;
        // 每次循环时都从新设置select监控信号集
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (errno == EINTR || errno == EWOULDBLOCK)
            continue;
        if (nready < 0)
        {
            error("select");
        }
        // 说明客户端有链接请求
        if (FD_ISSET(s_sock, &rset))
        {
            client_addr_len = sizeof(client_addr);
            // c_sock 客户端文件描述符
            c_sock = Accept(s_sock, (struct sockaddr *)&client_addr, &client_addr_len);
            // 向监控文件描述符集合allset添加新的文件描述符
            FD_SET(c_sock, &allset);
            // 判断客户端链接上限
            int p = 0;
            for (p = 0; p < FD_SETSIZE; p++)
            {
                if (client[p] < 0)
                {
                    client[p] = c_sock;
                    maxp = p + 1;
                    break;
                }
            }
            if (p == FD_SETSIZE)
            {
                cout << "客户端链接已达到上限" << endl;
                exit(1);
            }

            // 修改maxfd
            if (maxfd < c_sock)
            {
                maxfd = c_sock;
            }
            //  只有listen有事件，后续的置零无需执行
            if (0 == --nready)
            {
                continue;
            }
        }
        // 检测那个c_sock有数据就绪
        for (int i = s_sock + 1; i <= maxfd; i++)
        {
            c_sock = client[i];
            // 客户端字节符未在数组中则跳过
            if (c_sock = client[i] < 0)
            {
                continue;
            }
            // 如果是当前fd在读取
            if (FD_ISSET(i, &rset))
            {
                int read_len = 0;
                // read_len=0，代表客户端已经关闭
                read_len = Read(i, buf, sizeof(buf));
                // 说明客户端已经关闭
                if (read_len == 0)
                {
                    Close(i);
                    FD_CLR(i, &allset); // 接触select对文件描述符的监控
                    client[i] = -1;
                    maxp--;
                }
                else if (read_len > 0)
                {
                    for (int j = 0; j < read_len; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    Write(i, buf, read_len);
                    Write(STDOUT_FILENO, buf, read_len);
                }
            }
        }
    }

    // 关闭服务器socket
    Close(s_sock);
}