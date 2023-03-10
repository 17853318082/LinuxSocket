#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string>

#ifndef _WRAP_H_
#define _WRAP_H_

// state socket methods

void error(const std::string &str);            // error
void success(const std::string &str);           //success
int Socket(int domain, int type, int protocol);     //socket
int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);  //bind
int Listen(int sockfd, int backlog);                                 //listen
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);   //accept
ssize_t Read(int fd,void *ptr,size_t nbytes);                   // read
ssize_t Write(int fd,const void *ptr,size_t nbytes);
int Close(int fd);

#endif