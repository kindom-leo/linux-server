#ifndef __WRAP__H__
#define __WRAP__H__

#include "apue.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>

int Socket(int, int, int);
int Bind(int, const struct sockaddr_in*, socklen_t);
int Connect(int,const struct sockaddr_in*,socklen_t);
int Listen(int, int);
int Accept(int, struct sockaddr_in*, socklen_t*);
ssize_t Read(int, char*, size_t);
ssize_t Write(int, const char*, size_t);
int Close(int);
ssize_t Read_n(int, void*, size_t);
ssize_t Write_n(int, const void*, size_t);
static ssize_t my_read(int, char*);
ssize_t ReadLine(int, void*, size_t);
#endif //  __WRAP__H__
