//
// Created by binsus on 03/09/20.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <zconf.h>
#define BUFFSIZE 1024

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFSIZE];

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    printf("Enter message: \n");
    for(;;){
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
            error("Error connecting to socket");

        int index = 0;
        char word[1024], c;

        memset(word, 0, sizeof(word));
        do{
            c = (char)fgetc(stdin);
            word[index] = c;
            index++;
        } while(c != '\n');

        if(write(sockfd, word, strlen(word)) < 0)
            error("Error writing to socket");

        char recv[1024];
        memset(recv, 0, sizeof(recv));
        if(read(sockfd, recv, sizeof(recv)) < 0)
            error("Error reading from socket");

        printf("Replies from the server: %s", recv);
        close(sockfd);
    }
}

