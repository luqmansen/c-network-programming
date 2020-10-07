#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define listenQ 5
#define MAX_FILE_DESCRIPTOR 8

typedef struct sockaddr SA;

int main(int argc, char **argv)
{
    int     listenfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char    cliaddr_str[INET_ADDRSTRLEN];
    char message_buf[1024];
    char broadcast_buf[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // make two sets of file descriptors
    fd_set current_fd, ready_fd;

    FD_ZERO(&current_fd);
    FD_SET(listenfd, &current_fd);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons( 3002);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, listenQ) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for ( ; ; ) {

        // select() overwrites the socket set, so make a backup.
        ready_fd = current_fd;
        if (select(MAX_FILE_DESCRIPTOR, &ready_fd, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // check file descriptors one by one
        for (int i = 0; i < MAX_FILE_DESCRIPTOR; i++) {
            // if its ready to be read
            if (FD_ISSET(i, &ready_fd)) {
                // if it is the server's socket
                if (i == listenfd) {
                    len = sizeof(cliaddr);
                    connfd = accept(listenfd, (SA *) &cliaddr, &len);
                    if (connfd == -1) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    } else {
                        FD_SET(connfd, &current_fd);
                        // get client address
                        inet_ntop(AF_INET, &cliaddr.sin_addr, cliaddr_str, INET_ADDRSTRLEN);
                        printf("New connection from %s:%d will be handled by file descriptor %d\n", cliaddr_str, cliaddr.sin_port, connfd);
                    }
                } else { // if it is the socket to client
                    // get client address
                    inet_ntop(AF_INET, &cliaddr.sin_addr, cliaddr_str, INET_ADDRSTRLEN);

                    // if got error from client or client goes away

                    // Read bytes from client
                    long int message_length = read(i, message_buf, sizeof(message_buf));
                    if (message_length < 0) { // if got error from client (message length < 0)
                        perror("recv");
                        exit(EXIT_FAILURE);
                    } else if (message_length == 0) { // if client goes away (empty message)
                        printf("Connection closed from %s:%d\n", cliaddr_str, cliaddr.sin_port);
                        close(i);
                        FD_CLR(i, &current_fd);
                    } else { // if legit a message from client
                        connfd = i; // Set the file descriptor.

                        // Log the message
                        printf("Message from %s:%d -> %s", cliaddr_str, cliaddr.sin_port, message_buf);
                        // Format the will-be-broadcast message
                        strcat(broadcast_buf, cliaddr_str);
                        strcat(broadcast_buf, " -> ");
                        strcat(broadcast_buf, message_buf);

                        // Broadcast message to everyone
                        for (int idx=0; idx < MAX_FILE_DESCRIPTOR; idx++) {
                            if (FD_ISSET(idx, &ready_fd)) {
                                // Do not broadcast to server and to the client that sent the message
                                if (/*idx != listenfd && idx != i*/ 1) {
                                    int bytes_sent = write(idx, broadcast_buf, sizeof(broadcast_buf));
                                    if (bytes_sent == -1) {
                                        perror("send");
                                    } else {
                                        printf("%d bytes sent to %s:%d via file descriptor %d", bytes_sent, cliaddr_str, cliaddr.sin_port, idx);
                                    }
                                    close(idx);
                                    FD_CLR(idx, &current_fd);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
