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

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
       printf("cannot create a socket.");
       return 0;
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
    if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("ERROR: failed to set non-blocking");
        close(sockfd);
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) < 0)
    {
        perror("ERROR in inet_pton() :");
        close(sockfd);
        return 0;
    }

    server_len = sizeof(server_addr);

    while (fgets(send_buf, BUF_LEN, stdin) != NULL) 
    {
        while((sendto(sockfd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&server_addr, server_len)) == -1) 
        {
            if(errno == EWOULDBLOCK)
                continue;

            perror("Error in sending: ");
            close(sockfd);
        }

        while((recvlen = recvfrom(sockfd, recv_buf, BUF_LEN, 0, NULL, NULL)) == -1) 
        {
            if(errno == EWOULDBLOCK)
                continue;

            perror("Error in recv: ");
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
