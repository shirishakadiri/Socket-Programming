#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
    int socketClient,n;
    struct sockaddr_in clientAddress;
    
    //creating socket
    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    //check if socket has been created
    if (socketClient < 0)
        printf("cannot create socket");
    
    //creating address structure for socket
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddress.sin_port = htons(8888);
    
    //connect socket and check if connected
    if (connect(socketClient,(struct sockaddr *)&clientAddress,sizeof(clientAddress)) < 0)
        printf("connect failed");
    
    printf("Connection Successful, Start chat\n");
    char fromClient[1024], toClient[1024], buffer[1024];
    //sending and receiving messages from socket
    while(1)
    {
        
        memset(toClient, 0, sizeof toClient);
        memset(fromClient, 0, sizeof fromClient);
        
        if(fgets(fromClient,1024,stdin)){
            printf("Message to server : %s", fromClient);
            write(socketClient,fromClient,strlen(fromClient));
        }
        
        read(socketClient,toClient,1024);
            printf("Message from server : %s\n",toClient);
        
    }
    
    //close the socket
    close(socketClient);
    return 0;
}
