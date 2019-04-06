#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>


void *multipleClientsThreads(void *);

int main(int argc , char *argv[])
{
    int socketServer , clientSockets , c , *new_sock;
    struct sockaddr_in server , client;
    socklen_t addressLength = sizeof(struct sockaddr_in);
    
    //creating socket
    socketServer = socket(AF_INET , SOCK_STREAM , 0);
    //Check if the socket is created
    if (socketServer < 0 )
        printf("cannot create socket");
    
    //create address for socket
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    //bind to socket
    if( bind(socketServer,(struct sockaddr *)&server , sizeof(server)) < 0) {
        printf("cannot bind\n");
        return 1;
    }

    //Listen to the socket
    listen(socketServer , 3);
    
    //Accepting clients
    printf("Waiting for clients...\n");
    
    //accept client and create thread
    while( (clientSockets = accept(socketServer, (struct sockaddr *)&client, &addressLength))){
        
        //create thread
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = clientSockets;
        
        //create connection for each client
        if( pthread_create( &sniffer_thread , NULL , multipleClientsThreads , (void*) new_sock) < 0){
            printf("cannot create thread");
            return 1;
        }
    }
    
    if (clientSockets < 0){
        printf("cannot accept clients\n");
        return 1;
    }
    return 0;
}

//thread for each client and connect
void *multipleClientsThreads(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int n;
    char *message , fromServer[1024], toServer[1024];
    
    //reading and writing to socket
    while(1)
    {
    start: memset(toServer, 0, sizeof toServer);
        memset(fromServer, 0, sizeof fromServer);

        if(read(sock,toServer,1024)){
            printf("Message from client : %s",toServer);
        }
        fgets(fromServer,1024,stdin);
        printf("Message to client : %s", fromServer);
        write(sock,fromServer,strlen(fromServer));
    }
    
    //close socket
    close(sock);
    pthread_exit(NULL);
    return 0;
}
