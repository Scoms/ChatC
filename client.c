#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>

#define SERVER_ADDR     "127.0.0.1"     /* localhost */
#define MAXBUF          1024
#define PORT        3490

int main(int argc, char **argv)
{   
	int sockfd;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    /*---Open socket for streaming---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("Socket");
    }

    /*---Initialize server address/port struct---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    if ( inet_aton(SERVER_ADDR, &dest.sin_addr.s_addr) == 0 )
    {
        perror(SERVER_ADDR);
    }

    /*---Connect to server---*/
    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
    {
        perror("Connect ");
    }

    //bzero(buffer, MAXBUF);
    while(1)
    {
        fgets(buffer, MAXBUF, stdin);
        printf("%s %s","Envoie de la requete pour ",buffer);
        int res = send(sockfd, buffer,MAXBUF, 0);
    }

    /*---Clean up---*/
    close(sockfd);
    return 0;
}

