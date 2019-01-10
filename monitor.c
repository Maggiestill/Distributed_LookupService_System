#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define PORT_SERVER_A 21475
#define PORT_SERVER_B 22475
#define PORT_SERVER_C 23475
#define PORT_AWS_UDP 24475
#define PORT_AWS_TCP_C 25475
#define PORT_AWS_TCP_M 26475

int main(int inpnum, char*inp[])     
{
  int mfd;   /*socket*/                       
  char buffer[1024];

  /*initialiez*/
  struct sockaddr_in s_add,m_add;
  memset(&s_add,0,sizeof(s_add));
  s_add.sin_family=AF_INET;
  s_add.sin_addr.s_addr=inet_addr("127.0.0.1");
  s_add.sin_port=htons(PORT_AWS_TCP_M);

  /*create TCP socket*/
  if((mfd = socket(PF_INET,SOCK_STREAM,0))<0)      
  {
    perror("socket error:\n");
    return 1;
  }

  /*connect*/
  if(connect(mfd,(struct sockaddr*)(&s_add),sizeof(struct sockaddr))<0)   //cdf
  {
    close(mfd);
    perror("monitor: connect");
    if(connect(mfd,(struct sockaddr*)(&s_add),sizeof(struct sockaddr))<0)
    {perror("monitor connect error:");}
  }
  printf("The monitor is up and running.\n");


  /*receive result from aws via TCP*/
  if(recv(mfd,buffer,1024,0)<0)
  {
    perror("recv error:\n");
    return 1;
  }

  /*simple handle then show the result*/
  char *input= strtok(buffer,"@@");
  char *name_pm = strtok(NULL,"@@");
  //printf("namp aterfirstcut<%s>\n", name_pm);
  char *num = strtok(name_pm,"::");
  char *name_p = strtok(NULL,"::");
  //printf("name_p aterthirdcut<%s>\n", name_p);
  printf("Found <%s> match for <%s>\n", num,input);
  char *name_every = strtok(name_p,"&&");
  while(name_every)
  {   printf("<%s>\n",name_every);
      name_every = strtok(NULL,"&&");
  } 


  close(mfd);
  return 0;
}
