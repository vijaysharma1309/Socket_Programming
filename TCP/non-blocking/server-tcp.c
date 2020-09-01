/*
* Non-Blocking TCP Server example for learning purpose.
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

#define DEFAULT_PORT            55000
#define MAX_CLIENT_SUPPORTED    10
#define BUFFER_SIZE             256

/*An array of File descriptors */
int monitored_fd_set[MAX_CLIENT_SUPPORTED];

/*Each connected client's intermediate result is 
 * maintained in this client array.*/
int client_result[MAX_CLIENT_SUPPORTED] = {0};

/*Remove all the FDs, if any, from the the array*/
static void intitiaze_monitor_fd_set()
{
    int i;
    for(i = 0; i < MAX_CLIENT_SUPPORTED; ++i)
    {
        monitored_fd_set[i] = -1;
    }
}

/*Add a new FD to the monitored_fd_set array*/
static void add_to_monitored_fd_set(int socket_fd)
{
    int i;
    for(i = 0; i < MAX_CLIENT_SUPPORTED; ++i)
    {
        if(monitored_fd_set[i] != -1)
            continue;
        monitored_fd_set[i] = socket_fd;
        break;
    }
}

/*Remove the FD from monitored_fd_set array*/
static void remove_from_monitored_fd_set(int socket_fd)
{
    int i;
    for(i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if(monitored_fd_set[i] != socket_fd)
            continue;

        monitored_fd_set[i] = -1;
        break;
    }
}

/* Copy all the FDs from monitored_fd_set array to 
 * fd_set Data structure */
static void refresh_fd_set(fd_set *fd_set_ptr)
{
    FD_ZERO(fd_set_ptr);
    int i;
    for(i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if(monitored_fd_set[i] != -1)
        {
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

/* Get the maximum fd from the array */
static int get_max_fd()
{
    int i;
    int max = -1;

    for(i = 0; i < MAX_CLIENT_SUPPORTED; i++)
    {
        if(monitored_fd_set[i] > max)
            max = monitored_fd_set[i];
    }

    return max;
}

void close_socket(int sockfd, int conn_fd)
{
    close(sockfd);
    close(conn_fd);
}

int main()
{
    int sockfd, conn_fd, conn_socket_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t size = sizeof(client_addr);
    char buffer[256];
    size_t len;
    int i, ret = 0, sel_ret = 0;
    fd_set readfds;
    int flags = 0;
    struct timeval timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    intitiaze_monitor_fd_set();
    add_to_monitored_fd_set(0);

    /* Create a socket for communication */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create the socket\n");
        return -1;
    }

    /* non-blocking socket. 'fcntl' manipulates the file descriptor. */
    flags = fcntl(sockfd, F_GETFL, 0);
    if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("ERROR: failed to set non-blocking");
        close(sockfd);
        return 0;
    }

    /*Add master socket to Monitored set of FDs*/
    add_to_monitored_fd_set(sockfd);

    /* Initialize the server address struct with zeros */
    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family      = AF_INET;             /* using IPv4      */
    server_addr.sin_port        = htons(DEFAULT_PORT); /* on DEFAULT_PORT */
    server_addr.sin_addr.s_addr = INADDR_ANY;          /* from anywhere   */

    /* Bind the server socket to our port */
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
        fprintf(stderr, "ERROR: failed to bind\n");
        close(sockfd);
        return -1;
    }
    
    /* Listen for a new connection, allow 5 pending connections */
    if (listen(sockfd, 5) == -1) 
    {
        fprintf(stderr, "ERROR: failed to listen\n");
        close(sockfd);
        return -1;
    }

    /* This is the main loop for handling connections.
	 * Continue to accept clients until exit is issued. */
    while (1) 
    {
        refresh_fd_set(&readfds);

        sel_ret = select(get_max_fd() + 1, &readfds, NULL, NULL, &timeout);

        if(sel_ret < 0)
        {
            perror("ERROR in select(): ");
        }
        else if(sel_ret == 0)
        {   
            /* timeout */
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
        }
        else
        {
            if(FD_ISSET(sockfd, &readfds))
            {
                printf("Received new connection\n");
                memset(&client_addr, 0, sizeof(client_addr));

                conn_fd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
                if (conn_fd < 0)
                {
                    perror("ERROR: accept");
                    close(sockfd);
                    exit(EXIT_FAILURE);
                }

                printf("Connected to client\n");

                flags = fcntl(conn_fd, F_GETFL, 0);
                fcntl(conn_fd, F_SETFL, flags | O_NONBLOCK);

                add_to_monitored_fd_set(conn_fd);
            }
            else if(FD_ISSET(0, &readfds))
            {
                memset(buffer, 0, BUFFER_SIZE);
                read(0, buffer, BUFFER_SIZE);
                printf("Input read from console: %s\n", buffer);
            }
            else
            {
                conn_socket_fd = -1;
                for(i = 0; i < MAX_CLIENT_SUPPORTED; ++i)
                {
                    if(FD_ISSET(monitored_fd_set[i], &readfds))
                    {
                        conn_socket_fd = monitored_fd_set[i];

                        /*Prepare the buffer to recv the data*/
                        memset(buffer, 0, BUFFER_SIZE);

                        while((ret = read(conn_socket_fd, buffer, sizeof(buffer))) < 0)
                        {
                            if(errno == EWOULDBLOCK)
                                continue;

                            perror("ERROR: failed to read");
                            close_socket(sockfd, conn_socket_fd);
                            exit(EXIT_FAILURE);
                        }
                        buffer[ret] = '\0';
                        printf("message from client: %s\n", buffer);

                        if (write(conn_socket_fd, buffer, strlen(buffer)) != strlen(buffer)) 
                        {
                            if(errno == EWOULDBLOCK)
                                continue;

                            perror("ERROR: failed to write");
                            close_socket(sockfd, conn_socket_fd);
                            exit(EXIT_FAILURE);
                        }

                        if(!strncmp(buffer, "exit", 4))
                        {
                            printf("exiting\n");
                            remove_from_monitored_fd_set(conn_socket_fd);
                            close(conn_socket_fd);
                        }
                    }
                }
            }
        }
        
        
    }

    close(sockfd);          
    return 0;               
}
