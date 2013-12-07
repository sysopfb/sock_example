#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "base.h"

#define SERVER_IP "127.0.0.1"
#define PORT 5500

int server;

int client[2];

void *handle_client(void *data) {
	int *arg = (int *) data;
	int sockfd = *arg;
	FILE *fp;
	char msg[100];
	char line[100];

	for(;;)
	{
		recv(sockfd, msg, 1024, 0);
		printf("%s", msg);
		if(strncmp(msg, "quit", 4) == 0)
		{
			printf("Quit received\n");
			close(sockfd);
			pthread_exit(0);
		}
		else
		{
			printf("Else clause\n");
			if((fp = fopen(msg, "r")))
			{
				printf("Opening file %s\n", msg);
				while(fgets(line, 100, fp))
				{
					//remove_newline(line);
					write(sockfd, line, strlen(line));
				}
				fclose(fp);
			}
		}
	}
	pthread_exit(0);
}


int main()
{
	pthread_t client_handler;
	
	server = socket(AF_INET, SOCK_STREAM, 0);
	if(server == -1) die("can't create server socket");

	//
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	//if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	//	die("can't convert IP address");

	if(bind(server, (struct sockaddr *) &server_addr, sizeof server_addr) == -1)
		die("Can't bind server socket");

	listen(server, 10);

	int i = 0;
	char msg[512];
	for(;;)
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof client_addr;
		memset(msg, 0, 512);
		if(i<1)
		{
			int clientfd = accept(server, (struct sockaddr *) &client_addr, &client_len);
			printf("Client connected\n");
			if(pthread_create(&client_handler, NULL, handle_client, &clientfd) != 0)
			{
				die("couldn't create msg handler thread");
			}
			i++;
		} else {
			int noaccept = accept(server, (struct sockaddr *) &client_addr, &client_len);
			sprintf(msg, "SORRY WE ARE FULL");
			write(noaccept, msg, strlen(msg));
			close(noaccept);
		}
	}
	pthread_join(client_handler, NULL);

	return 0;
}

