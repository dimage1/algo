#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
   
#define PORT     8181 
   
// Driver code 
int main() { 
    int sockfd; 
    struct sockaddr_in     servaddr; 
   
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    memset(&servaddr, 0, sizeof(servaddr)); 
       
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    inet_aton ("127.0.0.1", &servaddr.sin_addr);

    //try to connect...
    int status = connect(sockfd, (sockaddr*) &servaddr, sizeof(servaddr));
    if(status < 0)
    {
        printf("Error connecting to socket!\n");
        return -1;
    }

    while (1) {
        const char *hello = "Hello from client";
        send(sockfd, (const char *)hello, strlen(hello), 0);
        std::cout << "Hello message sent."<< std::endl;

        sleep(1);
    }

    close(sockfd); 
    return 0; 
}