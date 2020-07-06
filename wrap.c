#include "wrap.h"
int Socket(int domain, int type, int proto){
	int n = 0;
	if ((n = socket(domain, type, proto)) < 0){
		perror("socket()");
		exit(n);
	}
	return n;
}

int Bind(int sockfd, const struct sockaddr_in* addr, socklen_t addrlen){
	int n = 0;
	if ((n = bind(sockfd, (struct sockaddr*)addr, addrlen)) < 0){
		perror("bind()");
		exit(n);
	}
	return n;
}

int Connect(int sockfd, const struct sockaddr_in* addr, socklen_t addrlen) {
	int n = 0;
	if ((n = connect(sockfd, (struct sockaddr*)addr, addrlen)) < 0){
		perror("connect()");
		exit(n);
	}
	return n;
}

int Listen(int sockfd, int backlog) {
	int n = 0;
	if ((n = listen(sockfd, backlog)) < 0){
		perror("listen()");
		exit(n);
	}
	return n;
}

int Accept(int sockfd, struct sockaddr_in* addr, socklen_t* addrlen) {
	int n = 0;
again:
	if ((n = accept(sockfd, (struct sockaddr*)addr, addrlen)) < 0){
		if (ECONNABORTED == errno || EINTR == errno){
			goto again;
		}
		perror("accept()");
		exit(n);
	}
	return n;
}

ssize_t Read(int fd, char* buf,	size_t count){
	ssize_t n;
again:
	if ((n = read(fd, buf, count)) < 0){
		if (EINTR == errno) {
			goto again;
		}else{
			return -1;
		}
	}
	return n;
}

ssize_t Write(int fd, const char* buf, size_t count) {
	ssize_t n;
again:
	if ((n = write(fd, buf, count)) < 0){
		if (EINTR == errno){
			goto again;
		}
	}
	return n;
}

int Close(int fd) {
	int n;
	if ((n = close(fd)) < 0 ){
		perror("close()");
		exit(-1);
	}
	return n;
}

ssize_t Read_n(int fd, void* buf, size_t n){
	size_t		nleft;
	ssize_t		nread;
	char*		ptr;
	ptr = (char*)buf;
	nleft = n;	

	while (nleft > 0){
		if ((nread = read(fd, ptr, nleft)) < 0){
			if (EINTR == errno){
				nread = 0;
			}else{
				return -1;
			}
		}else if (nread == 0){
			break;
		}
		nleft -= nread;
		ptr += nread;
	}
	return n - nleft;
}

ssize_t Write_n(int fd, const void* buf, size_t n){
	size_t    nleft;
	ssize_t	  nwritten;
	const char* ptr;

	nleft = n;
	ptr = (const char*)buf;
	
	while (nleft > 0){
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if (EINTR == errno && nwritten < 0){
				nwritten = 0;
			}else{
				return -1;
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

static ssize_t my_read(int fd, char* ptr){
	static int read_count;
	static char* read_ptr;
	static char read_buf[100];

	if (read_count <= 0){
again:
		if ((read_count = read(fd, read_buf, sizeof(read_buf))) < 0){
			if (EINTR == errno){
				goto again;
			}else {
				return -1;
			}
		}else if (read_count == 0){
			return 0;
		}
		read_ptr = read_buf;
	}
	read_count--;
	*ptr = *read_ptr++;
	return 1;
}


ssize_t ReadLine(int fd, void* buf, size_t len){
	ssize_t n, rc;
	char	c, *ptr;
	ptr = (char*)buf;

	for (n = 1; n <len; ++n){
		if ((rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n') {
				break;
			}
		}else if (rc == 0){
			*ptr = 0;
			return n - 1;
		}else {
			return -1;
		}
	}
	*ptr = 0;
	return n;
}










