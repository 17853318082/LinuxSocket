/*
  clent socket file
*/

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string>

using namespace std;

#define SERVER_PORT 8000     // server port define

void error(string str){
    cout<<str<<"  error"<<endl;
    exit(1);
}
void success(string str){
    cout<<str<<" success"<<endl;
}
void check(int c,string msg_c){
    /*  
       check run wither success
    */
    if(c == -1){
        error(msg_c);
    }else{
        success(msg_c);
    }
}

int main(){
    int cfd;  // client socket return value
    struct sockaddr_in s_addr; // server addr struct;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SERVER_PORT);  
    inet_pton(AF_INET,"127.0.0.0",&s_addr.sin_addr.s_addr);       // server ip 

    // 1.create client socket
    cfd = socket(AF_INET,SOCK_STREAM,0);
    check(cfd,"socket");
    // 2.connect server
    int check_connect = connect(cfd,(struct sockaddr*)&s_addr,sizeof(s_addr));   // the fist hand with server
    check(check_connect,"connect");
    // 3.toward server send request
    bool run = true;
    int i = 0;
    while(run){
        if(i>=10){
            run = false;
        }
        write(cfd,"hello",5);
        char buf[BUFSIZ];     // receive server response;
        int ret = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);
        cout<<endl;
        i++;
        sleep(1);
    }
    return 0;
}