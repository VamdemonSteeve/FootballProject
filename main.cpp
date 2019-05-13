#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define TRUE   1
#define FALSE  0
#define PORT 10007
int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[2] ,
    max_clients = 2 , activity, i , valread , sd, id[2], orientation[2],coord1[2],coord2[2],checker[2], speed[2],accelerator[2], ball[2], bcoord1[2], bcoord2[2], inertion[2];
    bool bkicked[2], bstandBy[2];
    bool change = false;
    int max_sd;
    struct sockaddr_in address;
    
    char buffer[1025],scoreA,scoreB;  //data buffer of 1K
    fd_set readfds;
    char *message = "ECHO Daemon v1.0 \r\n";
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);
    if (listen(master_socket, 2) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    
    while(TRUE)
    {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
        for ( i = 0 ; i < max_clients ; i++)
        {
            sd = client_socket[i];
            if(sd > 0)
                FD_SET( sd , &readfds);
            if(sd > max_sd)
                max_sd = sd;
        }
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }
            
            puts("Welcome message sent successfully");
            for (i = 0; i < max_clients; i++)
            {
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    
                    break;
                }
            }
        }
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            change = false;
            if (FD_ISSET( sd , &readfds))
            {
                read(client_socket[i], &id[i], sizeof(int));
                read(client_socket[i], &orientation[i], sizeof(int));
                read(client_socket[i],&accelerator[i],sizeof(int));
                read(client_socket[i], &coord1[i],sizeof(int));
                read(client_socket[i], &coord2[i],sizeof(int));
                read(client_socket[i], &ball[i], sizeof(int));
                read(client_socket[i], &bcoord1[i], sizeof(int));
                read(client_socket[i], &bcoord2[i], sizeof(int));
                read(client_socket[i], &bkicked[i], sizeof(bool));
                read(client_socket[i], &bstandBy[i], sizeof(bool));
                read(client_socket[i], &inertion[i], sizeof(int));
                
                write(client_socket[(i + 1) % 2], &ball[i], sizeof(int));
                write(client_socket[(i + 1) % 2], &bcoord1[i], sizeof(int));
                write(client_socket[(i + 1) % 2], &bcoord2[i], sizeof(int));
                write(client_socket[(i + 1) % 2], &bkicked[i], sizeof(bool));
                write(client_socket[(i + 1) % 2], &bstandBy[i], sizeof(bool));
                write(client_socket[(i + 1) % 2], &inertion[i],sizeof(int));
                write(client_socket[(i + 1) % 2], &id[i], sizeof(int));
                write(client_socket[(i + 1) % 2], &orientation[i], sizeof(int));
                write(client_socket[(i + 1) % 2],&accelerator[i],sizeof(int));
                write(client_socket[(i + 1) % 2], &coord1[i],sizeof(int));
                write(client_socket[(i + 1) % 2], &coord2[i],sizeof(int));
                printf("What i got is : %d,  %d,  %d,  %d,  %d \n",ball[i],bkicked[i],bstandBy[i],bcoord1[i], bcoord2[i]);
                id[i] = 0;
                orientation[i] = 0;
                accelerator[i] = 0;
                coord1[i] = 0;
                coord2[i] = 0;
                ball[i] = 0;
                
            }
        }
    }
    
    return 0;
}
