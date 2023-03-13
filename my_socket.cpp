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

ssize_t Readn(int fd,char *vptr,size_t n){

    size_t nleft=0;       // usigned int remain bytes
    ssize_t nread=0;      // int 
    char *ptr;

    ptr = vptr;
    nleft = n;           // all bytes number

    while(nleft>0){
        nread = read(fd,ptr,nleft);
        if(nread<0){
            if(errno == EINTR){
                nread = 0;
            }else{
                return -1;
            }
        }else if(nread == 0){
            break;
        }
        nleft -=nread;
        ptr +=nread;
    }

    return n-nleft;
}

ssize_t Write(int fd,const char* vptr,size_t n){
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while(nleft>0){      // have remain types
        nwritten = write(fd,ptr,nleft);
        if(nwritten<0){
            if(nwritten<0&&errno == EINTR){
                nwritten = 0;
            }else{
                return -1;
            }
        }
        nleft -=nwritten;
        ptr +=nwritten;
    }
    return n;
}

static ssize_t my_read(int fd, char *ptr){
	static int read_cnt;
	static char *read_ptr;
	static char read_buf[100];

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return -1;
		} else if (read_cnt == 0)
			return 0;
		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;

	return 1;
}

ssize_t Readline(int fd, char *vptr, size_t maxlen){
	ssize_t n, rc;
	char    c, *ptr;

	ptr = vptr;
	for (n = 1; n < (int)maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c  == '\n')
				break;
		} else if (rc == 0) {
			*ptr = 0;
			return n - 1;
		} else
			return -1;
	}
	*ptr  = 0;
	return n;
}
