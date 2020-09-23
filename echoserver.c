//
// Created by binsus on 03/09/20.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h> // for structure definition needed for socket
#include <netinet/in.h> // constant and struct for internet domain address
#include <strings.h>
#include <time.h>
#include <memory.h>
#include <netdb.h>
#include <zconf.h>
#define BUFFSIZE 1024

 //This function called to print error message when sys calls fails.
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    // Disable buffered output so we can print to console
    setvbuf (stdout, NULL, _IONBF, 0);

    // sockfd is a file descriptor, each running process has file desc which
    // contain a pointer to all i/o stream. Read the rest of the explanation here
    // https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/fd.txt
    int sockfd;
    // connfd is new file descriptor when client is connected to server. All
    // communication on this connection will use this new FD until conn is closed.
    int connfd;;
    // length of read/write buffer
    int n;
    // Server address of the server
    struct sockaddr_in	serv_addr;
    // Address of the client
    struct sockaddr_in	cli_addr;
    // Port number in which server is running
    int portno;
    // Clilen stores the size of the address of the client. This is needed for
    // the accept system call.
    int clilen;
    // Buffer to write response
    char buffer[BUFFSIZE];
    struct hostent *hostp; /* client host info */


    if (argc < 2){
        fprintf(stderr, "Usage: ./server.o <port>");
        exit(1);
    }

    // Create a syscall to create new socket, First args is domain type of the socket
    // there is two possible domain type (AF_INET or AF_UNIX). Second args os type of
    // socket, there is two options (SOCK_STREAM or SOCK_DGRAM). Use SOCK_STREAM for
    // TCP connection, SOCK_DGRAM for UDP connection. Third args is protocol, just set
    // it to 0, the OS will choose the appropriate protocol itself
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");
    printf("Opening socket\n");
    // Sets all values in buffer to zero
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Assign port number where the server will listen to
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    // This is for ip address of the host, for server code, this will be the IP of the
    // the machine on which the server is running. INADDR_ANY means, it's accept any
    // incoming message
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind syscall bind a socket to an address. This code bind current host and port
    // on the this server. Bind takes tree args, first is the socket file desc. Second args
    // is the address to which is bound. This is a pinter to a structure of type sockaddr,
    // but since we only have a struct of serv_addr (which is type of sockaddr_in, it has
    // to be casted to correct type. Third is the size of the addr to which it's bound
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0 )
        error("Error on binding to socket");

    printf("Server started at 0.0.0.0:%d\r\n", portno);

    // Listen sys call allow process to listen to socket for a connection. First args is
    // a file descriptor to a socket. Second args is the size of backlog queue (the number
    // of the connection can waiting while process is handling conn. 5 is maximum size permitted
    // by most system
    listen(sockfd, 5);

    // Infinite loop while listening for any connection from client
    for (;;){
        clilen = sizeof(cli_addr);
        // accept socket connection and to a connfd file descriptor
        connfd = accept(sockfd, (struct sockaddr*) &cli_addr,(socklen_t*) &clilen);
                if (connfd < 0)
                    error("Error accepting connection");

        hostp = gethostbyaddr((const char *)&cli_addr.sin_addr.s_addr,
                              sizeof(cli_addr.sin_addr.s_addr), AF_INET);
        printf("receiving connection from %s\n", hostp->h_name);
        bzero(buffer, BUFFSIZE);
        // read from new connfd file descriptor
        n = read(connfd, buffer, BUFFSIZE);
        if (n < 0)
            error("Error reading from socket");

        printf("server received %d bytes: %s\n", n, buffer);
        // write response to connection socket
        n = write(connfd, buffer, strlen(buffer));
        if (n < 0)
            error("Error writing to socket");

        // close the socket connection
        close(connfd);
    }
}