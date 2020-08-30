
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* socket includes */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define DEFAULT_PORT 55000

int main(int argc, char** argv)
{
    int sockfd, ret;
    struct sockaddr_in server_addr;
    char buff[256];
    size_t len;

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

    /* Initialize the server address struct with 0 */
    memset(&server_addr, 0, sizeof(server_addr));

    /* Fill in the server address */
    server_addr.sin_family = AF_INET;             /* using IPv4      */
    server_addr.sin_port   = htons(DEFAULT_PORT); /* on DEFAULT_PORT */
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1) 
    {
        fprintf(stderr, "ERROR: invalid address\n");
        close(sockfd);
        return 0;
    }

    /* Connect to the server */
    if ( connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) 
    {
        fprintf(stderr, "ERROR: failed to connect\n");
        close(sockfd);
        return 0;
    }

    while(1)
    {
        printf("Message for server: ");
        memset(buff, 0, sizeof(buff));
        if (fgets(buff, sizeof(buff), stdin) == NULL) 
        {
            fprintf(stderr, "ERROR: failed to get message for server\n");
            close(sockfd);
            break;
        }
        len = strnlen(buff, sizeof(buff));

        if (write(sockfd, buff, len) != len) 
        {
            fprintf(stderr, "ERROR: failed to write\n");
            close(sockfd);
            break;
        }

        memset(buff, 0, sizeof(buff));
        if (read(sockfd, buff, sizeof(buff)-1) == -1)
        {
            fprintf(stderr, "ERROR: failed to read\n");
            close(sockfd);
            break;
        }

        printf("reply from Server: %s\n", buff);
        if(!strncmp(buff, "exit", 4))
        {
            break;
        }
    }

    close(sockfd);          

    return 0;
}
