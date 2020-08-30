
/* the usual suspects */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* socket includes */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

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

/* Get the maximum fd from the array*/
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
    int i, ret = 0;
    fd_set readfds;

    intitiaze_monitor_fd_set();
    add_to_monitored_fd_set(0);

    /* Create a socket for communication */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: failed to create the socket\n");
        return -1;
    }

    /*Add master socket to Monitored set of FDs*/
    add_to_monitored_fd_set(sockfd);

    /* Initialize the server address struct with zeros */
    memset(&server_addr, 0, sizeof(server_addr));

    /* Fill in the server address */
    server_addr.sin_family      = AF_INET;             
    server_addr.sin_port        = htons(DEFAULT_PORT); 
    server_addr.sin_addr.s_addr = INADDR_ANY;          

    /* Bind the server socket to our port */
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
    {
        fprintf(stderr, "ERROR: failed to bind\n");
        return -1;
    }

    /* Listen for a new connection, allow 5 pending connections */
    if (listen(sockfd, 5) == -1) 
    {
        fprintf(stderr, "ERROR: failed to listen\n");
        return -1;
    }

    while (1) 
    {
        refresh_fd_set(&readfds);
        printf("Waiting on select()\n");

        select(get_max_fd() + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &readfds))
        {
            /* New connection will come through this socket */
            printf("Received new connection\n");

            conn_fd = accept(sockfd, (struct sockaddr*)&client_addr, &size);

            if (conn_fd == -1) 
            {
                perror("accept");
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            add_to_monitored_fd_set(conn_fd);
        }
        else if(FD_ISSET(0, &readfds))
        {
            /* If the server gets input from its console, data coming from stdin(fd = 0) */
            memset(buffer, 0, BUFFER_SIZE);
            ret = read(0, buffer, BUFFER_SIZE);
            printf("Input read from console: %s\n", buffer);
        }
        else
        {
            /* Socket activated from the client side, data coming from client side */
            conn_socket_fd = -1;
            for(i = 0; i < MAX_CLIENT_SUPPORTED; ++i)
            {
                if(FD_ISSET(monitored_fd_set[i], &readfds))
                {
                    conn_socket_fd = monitored_fd_set[i];

                    memset(buffer, 0, BUFFER_SIZE);

                    if ((ret = read(conn_socket_fd, buffer, sizeof(buffer))) == -1) 
                    {
                        fprintf(stderr, "ERROR: failed to read\n");
                        close_socket(sockfd, conn_socket_fd);
                        exit(EXIT_FAILURE);
                    }
                    buffer[ret] = '\0';
                    printf("message from client: %s\n", buffer);
                   
                    if (write(conn_socket_fd, buffer, strlen(buffer)) != strlen(buffer)) 
                    {
                        fprintf(stderr, "ERROR: failed to write\n");
                        close_socket(sockfd, conn_socket_fd);
                        exit(EXIT_FAILURE);
                    }
                    
                    if(!strncmp(buffer, "exit", 4))
                    {
                        printf("exiting from this client\n");
                        remove_from_monitored_fd_set(conn_socket_fd);
                        close(conn_socket_fd);
                    }
                }
            }
        }
    }

    close(sockfd);          
    return 0;               
}
