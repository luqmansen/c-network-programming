#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <memory.h>
#include <netdb.h>
#include <zconf.h>

#define BUFFSIZE 1024

void error(char *msg);
void process_request(int);

int main(int argc, char *argv[]){
    setvbuf (stdout, NULL, _IONBF, 0);
    int sockfd, connfd, portno, clilen;
    struct hostent *hostp;
    struct sockaddr_in	serv_addr;
    struct sockaddr_in	cli_addr;

    if (argc < 2){
        fprintf(stderr, "Usage: ./echoserverfork.o <port>");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0 )
        error("Error on binding to socket");

    printf("Server started at 0.0.0.0:%d\r\n", portno);

    listen(sockfd, 5);

    for (;;){
        clilen = sizeof(cli_addr);
        connfd = accept(sockfd, (struct sockaddr*) &cli_addr,(socklen_t*) &clilen);
        if (connfd < 0)
            error("Error accepting connection");

        hostp = gethostbyaddr((const char *)&cli_addr.sin_addr.s_addr,
                              sizeof(cli_addr.sin_addr.s_addr), AF_INET);
        printf("receiving connection from %s\n", hostp->h_name);

        if (fork() == 0) {
            process_request(connfd);
        } else {
            close(connfd);
        }
    }
}


void error(char *msg){
    perror(msg);
    exit(1);
}

void process_request(int connfd){
    int n;
    char buffer[BUFFSIZE];

    bzero(buffer, BUFFSIZE);
    n = read(connfd, buffer, BUFFSIZE);
    if (n < 0)
        error("Error reading from socket");

    printf("processing request from pid: %d\n", getpid());
    printf("server received %d bytes: %s\n", n, buffer);

    n = write(connfd, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");
    close(connfd);
}