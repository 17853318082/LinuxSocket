/*
   server socket file
*/
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>

using namespace std;

void error(const string &s){
    cout<<s<<" error!"<<endl;
    exit(-1);
}
void success(const string &s){
    cout<<s<<" success"<<endl;
}

int main(){
    int s_listen;   // server socket
    int s_client;   // accept client socket request
    int check_bind;  //check bind
    int check_listen;
    bool run = true;
    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;  // client addr
    socklen_t c_addr_len = sizeof(c_addr);   
    char buf[BUFSIZ],s_ip[BUFSIZ],c_ip[BUFSIZ];
    s_addr.sin_port =htons(8000);
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // any ip addr;
    cout<<"server-ip: "<<inet_ntop(AF_INET,&s_addr.sin_addr.s_addr,s_ip,sizeof(s_ip))<<" "<<"s_port: "<<ntohs(s_addr.sin_port)<<endl; // print server ip and port

    // create socket
    s_listen = socket(AF_INET,SOCK_STREAM,0);
    if(s_listen == -1){
        error("socket");
        return 0;
    }else{
        success("socket");
    }

    // bind socket
    check_bind = bind(s_listen,(struct sockaddr*) &s_addr,sizeof(s_addr));
    if(check_bind == -1){
        error("bind");
        return 0;
    }else{
        success("bind");
    }

    // listen client request
    check_listen = listen(s_listen,128);
    if(check_listen == -1){
        error("listen");
        return 0;
    }else{
        success("listen");
    }

    // accept client request
    s_client = accept(s_listen,(struct sockaddr*)&c_addr, &c_addr_len);
    cout<<"client-ip: "<<inet_ntop(AF_INET,&c_addr.sin_addr.s_addr,c_ip,sizeof(c_ip))<<" "<<"c_port: "<<ntohs(c_addr.sin_port)<<endl;    // print client ip and port
    cout<<"listen client reqeust"<<endl;
    // accept client message
    while(run){
        int request_len = read(s_client,&buf,BUFSIZ);
        cout<<"request_len:"<<request_len<<"  request:"<<buf<<endl;
        for(int i = 0;i<request_len;i++){
            buf[i] = toupper(buf[i]);
        }
        // int response_len = send(s_client, &buf, request_len, 1);  // send method one
        cout<<"response_len:"<<request_len<<" "<<"content:"<<buf<<endl;  // send method two
        write(s_client,buf,request_len);
        success("response");
    }

    // close socket link
    close(s_listen);
    close(s_client);
    
    return 0;
}