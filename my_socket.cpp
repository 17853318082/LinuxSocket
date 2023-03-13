/*
  
*/
#include "my_socket.h"

// return error
void error(const std::string &str){
    std::cout<<str<<" error"<<std::endl;
    exit(-1);
}
//return success
void success(const std::string &str){
    std::cout<<str<<" success"<<std::endl;
}

// socket
int Socket(int domain, int type, int protocol){
    int n = 0;
    n = socket(domain,type,protocol);
    if(n==-1){
        error("socket");
        return n;
    }
    success("socket");
    return n;
}
// bind
int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen){
    int n = 0;
    n = bind(sockfd,addr,addrlen);
    if(n==-1){
        error("bind");
        return n;
    }
    success("bind");
    return n;
}

//listen
int Listen(int sockfd, int backlog){
    int n=0;
    n = listen(sockfd,backlog);
    if(n==-1){
        error("listen");
    }
    success("listen");
    return n;
}
// accept
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int n;
    n = accept(sockfd,addr,addrlen);
    if(n==-1){
        error("accept");
    }
    success("accept");
    return n;
}
