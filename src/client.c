#include "snakeEater.h"

int main(int argc, char const *argv[]) 
{ 
    int testSock, readVal; 
    struct sockaddr_in servAddress; 
    char *testMsg = "From client"; 
    char readBuffer[1024] = {0}; 
    testSock = socket(AF_INET, SOCK_STREAM, 0);
   
    servAddress.sin_family = AF_INET; 
    servAddress.sin_port = htons(PORTNUM); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    inet_pton(AF_INET, "127.0.0.1", &servAddress.sin_addr);
   
    connect(testSock, (struct sockaddr *)&servAddress, sizeof(servAddress));

    send(testSock , testMsg , strlen(testMsg) , 0 ); 
    printf("Sent message to server\n"); 
    read( testSock , readBuffer, 1024); 
    printf("Recieved: %s\n",readBuffer ); 
    return 0; 
} 