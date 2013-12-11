#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 

#define MON_PORT 3490
#define BACKLOG 10
#define MAXBUFF 1024
#define TRUE 1
#define FALSE 0

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

int main()
{
  int sockfd, new_fd, sin_size;
  struct sockaddr_in localadrr; /* Adresse maison */
  struct sockaddr_in their_addr; /* Adresse du contrevenant*/
  char * buffer = malloc(MAXBUFF);

  localadrr.sin_family = AF_INET;
  localadrr.sin_port = htons(MON_PORT);
  localadrr.sin_addr.s_addr = INADDR_ANY; /* laisse syst mettre mon addresse */
  bzero(&(localadrr.sin_zero),8); /* bourrage de la strcuture*/

  //Instanciation de la socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
  {
    perror("socket");
  }
  
  // connexion par les internets
  if(-1 == bind(sockfd,(struct sockaddr *)&localadrr, sizeof(localadrr)))
  {
    perror("error bind failed");
    close(sockfd);
  }

  // hey listenstruct sockaddr_in addr;
  if(listen(sockfd,BACKLOG) == -1 )
  {
      perror("listen");
  }

  while(1)
  {
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(MON_PORT);
    their_addr.sin_addr.s_addr = INADDR_ANY;
    sin_size = sizeof(struct sockaddr_in);
    
    if ((new_fd = accept(sockfd,(struct sockaddr *)&their_addr, &sin_size)) == -1)
    {
      perror("accept");
    }
    
    printf("serveur: Reçu connexion  de %s\nEn attente de la requète : \n",inet_ntoa(their_addr.sin_addr));  
    int founded = FALSE;
    while(!founded){
      recv(new_fd, buffer, MAXBUFF,0);
      buffer = trim(buffer);
      printf("%s%s\n","Recherche du fichier : ",buffer);
      FILE * file = fopen(buffer,"r");
      char str[999];
      if (file) {
        printf("file found\n");
          while (fscanf(file, "%s", str)!=EOF)
          {
              printf("read");
              printf("%s",str);
          }
          printf("----EOF---");
          fclose(file);

      }
    }
    close(new_fd);
    printf("Connexion fermée\n");  
  }
  return 0;
}
