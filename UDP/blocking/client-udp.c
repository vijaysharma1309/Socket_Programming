/*
* Blocking UDP Client example for learning purpose.
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

#define BUF_LEN   256
#define SERV_PORT 50000

int main(int argc, char** argv)
{
    int sockfd;
    int recvlen;
    struct sockaddr_in server_addr;
    socklen_t server_len;
    char send_buf[BUF_LEN];
    char recv_buf[BUF_LEN + 1];

    if (argc != 2) 
    {
        printf("usage: %s <IP address>\n", argv[0]);
        return 0;
    }

	/* Create a socket for UDP communication */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
       printf("cannot create a socket.");
       return 0;
    }

	/* Initialize the client address struct with zeros */
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) == -1)
    {
        perror("ERROR in inet_pton() :");
        close(sockfd);
        return 0;
    }

    server_len = sizeof(server_addr);

    while (fgets(send_buf, BUF_LEN, stdin) != NULL) 
    {
        if ((sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&server_addr, server_len)) == -1) 
        {
            perror("Error in sending: ");
            close(sockfd);
            break;
        }

        if ((recvlen = recvfrom(sockfd, recv_buf, BUF_LEN, 0, NULL, NULL)) == -1) 
        {
            perror("Error in recv: ");
            close(sockfd);
            break;
        }

        recv_buf[recvlen] = '\0';
    
        printf("Message from server: %s\n", recv_buf);
        if(!strncmp(send_buf, "exit", 4))
        {
            break;
        }
    }

    close(sockfd);

    return 0;
}
