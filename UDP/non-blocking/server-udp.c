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

#define SERV_PORT   50000                   
#define BUF_LEN      256                    

int main (void)
{
    int sockfd;                   
    int recvLen;           
    unsigned char buf[BUF_LEN];              
    struct sockaddr_in server_addr;     
    struct sockaddr_in client_addr;      
    socklen_t client_addrLen = sizeof(client_addr); 

    /* create a UDP/IP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("ERROR cannot create socket");
        return 0;
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
    if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("ERROR: failed to set non-blocking");
        close(sockfd);
        return 0;
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(SERV_PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("ERROR bind failed: ");
        close(sockfd);
        return 0;
    }

    while(1)
    {        
        while((recvLen = recvfrom(sockfd, buf, BUF_LEN, 0, (struct sockaddr *)&client_addr, &client_addrLen)) < 0)
        {
            if(errno == EWOULDBLOCK)
                continue;

            perror("ERROR in recvfrom(): ");
            close(sockfd);
        }

        buf[recvLen] = '\0';

        printf("Message from client port: %d\n", client_addr.sin_port);
        printf("%s\n", buf);

        while(sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, client_addrLen) < 0)
        {
            if(errno == EWOULDBLOCK)
                continue;

            perror("ERROR in sendto(): ");
            close(sockfd);
        }  
    }

    close(sockfd);
    return 0;
}

