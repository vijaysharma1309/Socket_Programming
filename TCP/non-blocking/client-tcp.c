/*
* Non-Blocking TCP Client example for learning purpose.
*/

/* the usual suspects */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* socket includes */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>

#define DEFAULT_PORT 55000

int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buff[256];
    size_t len;
    int flags = 0;

    /* Check if server address is given */
    if (argc != 2) 
    {
        printf("Usage: %s <IPv4 address>\n", argv[0]);
        return 0;
    }

    /* Create a socket for communication */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        fprintf(stderr, "ERROR: failed to create the socket\n");
        return 0;
    }

	/* non-blocking socket. 'fcntl' manipulates the file descriptor. */
    flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) 
    {
        perror("ERROR: failed to set non-blocking");
        close(sockfd);
        return -1;
    }

    /* Initialize the server address struct with 0 */
    memset(&server_addr, 0, sizeof(server_addr));

    /* Fill in the server address */
    server_addr.sin_family = AF_INET;             /* using IPv4      */
    server_addr.sin_port   = htons(DEFAULT_PORT); /* on DEFAULT_PORT */

    /* Get the server IPv4 address from the command line call */
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1) 
    {
        fprintf(stderr, "ERROR: invalid address\n");
        close(sockfd);
        return 0;
    }

    /* Connect to the server */
    while (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) 
    {
		/* Todo */
    }

    while(1)
    {
        printf("Message for server: ");
        memset(buff, 0, sizeof(buff));
        if (fgets(buff, sizeof(buff), stdin) == NULL) 
        {
            fprintf(stderr, "ERROR: failed to get message for server\n");
            close(sockfd);
            return 0;
        }
        len = strnlen(buff, sizeof(buff));

        while(write(sockfd, buff, len) != len) 
        {
            if(errno == EWOULDBLOCK)
                continue;

            fprintf(stderr, "ERROR: failed to write\n");
            close(sockfd);
            return 0;
        }

        memset(buff, 0, sizeof(buff));

        while(read(sockfd, buff, sizeof(buff)-1) == -1)
        {
            if(errno == EWOULDBLOCK)
                continue;

            fprintf(stderr, "ERROR: failed to read\n");
            close(sockfd);
            return 0;
        }

        printf("Server: %s\n", buff);
        if(!strncmp(buff, "exit", 4))
        {
            break;
        }
    }

    close(sockfd);          

    return 0;
}
