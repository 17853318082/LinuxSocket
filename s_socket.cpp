#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>

using namespace std;

void error(const string &s){
    cout<<s<<" error!"<<endl;
}
void success(const string &s){
    cout<<s<<" success"<<endl;
}

int main(){
    int s_listen;   // server socket
    int s_client;   // accept client request
    int check_bind;  //check bind
    int check_listen;
    bool run = true;
    struct sockaddr_in addr;
    struct sockaddr_in c_addr;  // client addr
    socklen_t c_addr_len = sizeof(c_addr);   
    char buf[BUFSIZ];
    addr.sin_port =htons(8001);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   // any ip addr
    // create socket
    s_listen = socket(AF_INET,SOCK_STREAM,0);
    if(s_listen == -1){
        error("socket");
    }else{
        success("socket");
    }

    // bind socket
    check_bind = bind(s_listen,(struct sockaddr*) &addr,sizeof(addr));
    if(check_bind == -1){
        error("bind");
    }else{
        success("bind");
    }
    // listen client request
    check_listen = listen(s_listen,128);
    if(check_listen == -1){
        error("listen");
    }else{
        success("listen");
    }

    // accept client request
    s_client = accept(s_listen,(struct sockaddr*)&c_addr, &c_addr_len);
    cout<<"listen client reqeust"<<endl;
    // accept client message
    while(run){
        int request_len = read(s_client,&buf,BUFSIZ);
        cout<<buf<<endl;
        // transform
        // for(int i = 0;i<request_len;i++){
        //     buf[i] = toupper(buf[i]);
        // }
        // int response_len = send(s_client, &buf, request_len, 1);
        write(s_client,buf,request_len);
        success("response");

    }
    // close socket link
    close(s_listen);
    close(s_client);
    
    return 0;
}