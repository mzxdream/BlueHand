#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVER_PORT 1018
#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
	return 0;
    }
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
    if (bind(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr)) == -1)
    {
	return 0;
    }
    if (listen(sockfd, 5) == -1)
    {
	return 0;
    }
    while (true)
    {
	int acceptfd = accept(sockfd, NULL, NULL);
	if (acceptfd <0)
	{
	    continue;
	}
	
    }
    return 0;
    
}
