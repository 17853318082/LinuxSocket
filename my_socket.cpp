/*
  pakage socket method
*/
#include "my_socket.hpp"

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
// read bytes
ssize_t Read(int fd,void *ptr,size_t nbytes){
    ssize_t n=0;

    n = read(fd,ptr,nbytes);
    if(n==-1){
        error("read");
        return n;
    }
    success("read");
    return n;
}

//write bytes
ssize_t Write(int fd,const void *ptr,size_t nbytes){
    ssize_t n =0;

    n = write(fd,ptr,nbytes);
    if(n==-1){
        error("write");
        return n;
    }
    return n;
}
// close socket byte simbol
int Close(int fd){
    int n =0;
    n = close(fd);
    if(n==-1){
        error("close");
        return n;
    }
    success("close");
    return n;
}

ssize_t Readn(int fd,void *vptr,size_t n){

    size_t nleft=0;       // usigned int remain bytes
    ssize_t nread=0;      // int 

    return nleft;
}