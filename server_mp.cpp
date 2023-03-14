#include <iostream>
#include <string.h>
#include <string>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "my_socket.cpp"

#define SERVER_PORT 9000

void catch_child(int singnum)
{
    while ((waitpid(0, NULL, WNOHANG) > 0))
        ; // huishou child precess
    return;
}

int main()
{
    int s_sock, c_sock;
    struct sockaddr_in s_addr, c_addr;
    socklen_t c_addr_len;
    int pid;
    int ret;
    char buf[BUFSIZ];
    c_addr_len = sizeof(c_addr);
    memset(&s_addr, 0, sizeof(s_addr)); // 将地址结构清零
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SERVER_PORT);
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // socket
    s_sock = Socket(AF_INET, SOCK_STREAM, 0);

    // bind
    Bind(s_sock, (struct sockaddr *)&s_addr, sizeof(s_addr));

    // listen
    Listen(s_sock, 128);

    // consist accept
    bool run = true;
    while (run)
    {
        // wait client request
        c_sock = Accept(s_sock, (struct sockaddr *)&c_addr, &c_addr_len);
        // create muti thread
        pid = fork(); // 开启一个子进程
        if (pid < 0)
        {
            error("fork"); // thread create error
        }
        else if (pid == 0)
        {                  // 成功创建子进程，结束掉while循环，执行接下来的代码
            close(s_sock); // 子进程中的服务器socket
            run = false;
            continue;
        }
        else
        {
            struct sigaction act; //
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            sigaction(SIGCHLD, &act, NULL);
            if (ret != 0)
            {
                error("singaction");
            }
            close(c_sock);
            continue;
        }
    }
    if (pid == 0)
    { // 子进程的执行
        bool read_run = true;
        while (read_run)
        {
            ret = Read(c_sock, buf, sizeof(buf)); // 读取客户端传过来的数据
            if (ret == 0)
            {
                close(c_sock);
            }
            for (int i = 0; i < ret; i++)
            {
                buf[i] = toupper(buf[i]);
            }
            write(c_sock, buf, ret);        // 向客户端发送数据
            write(STDOUT_FILENO, buf, ret); // 显示到终端中
        }
    }
    close(s_sock);
    return 0;
}