#include "apue.h"
#include "wrap.h"
#include <sys/socket.h>
#include <ctype.h>

#define SERV_PORT 6666
#define SERV_IP "127.0.0.1"
int main(){
	sockaddr_in client_addr, server_addr;

	int sock_server, sock_client;
	sock_server = Socket(AF_INET, SOCK_STREAM, 0);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, SERV_IP, &server_addr.sin_addr.s_addr);

	Bind(sock_server, &server_addr, sizeof(server_addr));
	Listen(sock_server, SOMAXCONN);

	int fd_max = sock_server;
	int i_max = -1;
	int fd_client[FD_SETSIZE];
	for (int i = 0;i < FD_SETSIZE; ++i){
		fd_client[i] = -1;
	}

	fd_set set_read, set_all;
	FD_ZERO(&set_all);
	FD_SET(sock_server, &set_all);

	int nready;
	socklen_t client_addr_len;
	char addrBuf[256];
	while (1){
		set_read = set_all;
		nready = select(fd_max + 1, &set_read, NULL, NULL, NULL);
		if (nready < 0) {
			perror("select()");
			exit(-1);
		}
		if (FD_ISSET(sock_server, &set_read)){
			client_addr_len = sizeof(client_addr);	
			sock_client = Accept(AF_INET, &client_addr, &client_addr_len);
			printf("RECV FROM '%s' at PORT: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, 
						addrBuf, sizeof(addrBuf)), ntohs(client_addr.sin_port));
			int i;
			for (i = 0;i < FD_SETSIZE; ++i){
				if (fd_client[i] < 0){
					fd_client[i] = sock_client;
					break;
				}
			}
			if (i == FD_SETSIZE){
				fprintf(stderr, "too many clients!\n");
				exit(-1);
			}

			FD_SET(sock_client, &set_all);
			if (sock_client > fd_max){
				fd_max = sock_client;
			}
			if (i > i_max){
				i_max = i;
			}

			if (--nready == 0){
				break;
			}
		}
		int sockfd;
		int n;
		char buf[BUFSIZ];
		for (int i = 0;i <= i_max; ++i){
			if ((sockfd = fd_client[i]) < 0){
				continue;
			}
			if (FD_ISSET(sockfd, &set_read)){
				if ((n = Read(sockfd, buf, sizeof(buf))) == 0){
					Close(sockfd);
					FD_CLR(sockfd, &set_all);
					fd_client[i] = -1;
				} else if (n > 0){
					for (int j = 0;j < BUFSIZ; ++j){
						buf[j] = toupper(buf[j]);
					}
					sleep(2);
					Write(sockfd, buf, n);
				}
				if (--nready == 0){
					break;
				}
			}
		}
	}
	close(sock_client);
	exit(0);
}



