#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>

#define PORT     8181
#define MAXLINE  1024

void worker(int clientfd) {
    char buffer[MAXLINE]; 

    printf("! Starting worker for client %d\n", clientfd); 

    while(1){
        int n = recv(clientfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            close(clientfd);
            break;
        }
        buffer[n] = '\0'; 
        printf("Client : %s\n", buffer); 
    }

    printf("! Ending worker for client %d\n", clientfd); 
}

// Driver code 
int main() { 
    int sockfd; 
    const char *hello = "Hello from server"; 
    struct sockaddr_in servaddr; 
       
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    memset(&servaddr, 0, sizeof(servaddr)); 
       
    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    //servaddr.sin_addr.s_addr = INADDR_ANY; 
    inet_aton ("127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT); 
       
    // Bind the socket with the server address 
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    listen(sockfd, 5);


    printf("Waiting for clients...\n");
    while (1) {
        struct sockaddr_in servaddr, clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t len = sizeof(clientAddr);

        int clientfd = accept(sockfd, (struct sockaddr *) &clientAddr, &len);
        if (clientfd < 0) {
            continue;
        }

        std::thread(worker, clientfd).detach();
        /*int n = recv(clientfd, buffer, sizeof(buffer), 0);
        if (n > 0) {
            buffer[n] = '\0'; 
            printf("Client : %s\n", buffer); 
        }
        close(clientfd);*/
    }

}