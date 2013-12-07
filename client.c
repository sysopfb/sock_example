#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "base.h"

#define SERVER_IP "127.0.0.1"
#define PORT 5500

int client;

int main()
{
	char msg[1024];

	client = socket(AF_INET, SOCK_STREAM, 0);
	if(client == -1)
		die("Socket");

	struct sockaddr_in address;
	memset(&address, 0, sizeof address);
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	if(inet_pton(AF_INET, SERVER_IP, &address.sin_addr) <= 0)
		die("IP CONVERSION");

	if(connect(client, (struct sockaddr *) &address, sizeof address) == -1)
		die("Can't connect to server");


	struct pollfd pfds[] = {{STDIN_FILENO, POLLIN, 0}, {client, POLLIN, 0}};

	for(;;)
	{
		switch(poll(pfds, 2, -1))
		{
			case -1:
				perror("poll");
				exit(EXIT_FAILURE);
			default:
				memset(msg, 0, 1024);
				if((pfds[0].revents & POLLIN) != 0)
				{
					fgets(msg, 1024, stdin);
					remove_newline(msg);
					write(client, msg, strlen(msg));
					if(strncmp(msg, "quit", 4) == 0)
						goto done;
				}
				if((pfds[1].revents & POLLIN) != 0)
				{
					read(client, msg, 1024);
					printf("%s", msg);
				}
		}
	}

	done:
	close(client);
	return 0;
}

