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
  int cfd;  /*socket*/                          
  char buffer[1024];

  /*initialization*/
  struct sockaddr_in s_add,c_add;
  memset(&s_add,0,sizeof(s_add));
  s_add.sin_family=AF_INET;
  s_add.sin_addr.s_addr=inet_addr("127.0.0.1");
  s_add.sin_port=htons(PORT_AWS_TCP_C);

  /*whether number of input is 2*/
  if(inpnum != 3)
  { 
    perror("number of input error:\n");
    return 1;
  }

  /*whether input function is search prefix*/
  if((strcmp(inp[1],"search")&&strcmp(inp[1],"prefix")) != 0)
  { 
    perror("funciont error:\n");
    return 1;
  }

  /*create TCP socket*/
  if((cfd = socket(PF_INET,SOCK_STREAM,0))<0)     
  {
    perror("socket error:\n");
    return 1;
  }

  /*connect socket*/
  if(connect(cfd,(struct sockaddr*)(&s_add),sizeof(struct sockaddr))<0)  
  {
    close(cfd);
    perror("client: connect");
    if(connect(cfd,(struct sockaddr*)(&s_add),sizeof(struct sockaddr))<0)
    {perror("client connect error:");}
  }
  printf("The client is up and running.\n");


  /*combining all input into one char*/
  char Message[1024];
  memset(&Message,'\0',sizeof(Message));
  sprintf(Message,"%s %s",inp[1],inp[2]);

  if((send(cfd,Message,sizeof(Message),0))<0)   /*send*/
  {
    perror("send error:\n");
    return 1;
  }
 
  printf("The client send <%s> and <%s> to AWS\n",inp[2],inp[1]);

  if(recv(cfd,buffer,1024,0)<0) /*receive result from aws via TCP*/
  {
    perror("recv error:\n");
    return 1;
  }

  /*for search function*/
  if(strcmp(inp[1],"search") == 0)
  { 
     char *nums= strtok(buffer,"::");
     char *name_s = strtok(NULL,"::");
     //printf("name_s <%s>\n", name_s);
     printf("Found <%s> match for <%s>\n", nums,inp[2]);
     char *name_everys = strtok(name_s,"&&");
     while(name_everys)
     {printf("<%s>\n",name_everys);
      name_everys = strtok(NULL,"&&");
     } 
  }

  /*for prefix fuction*/
  else
  { 
     char *num= strtok(buffer,"::");
     char *name_p = strtok(NULL,"::");
     //printf("name_p <%s>\n", name_p);
     printf("Found <%s> match for <%s>\n", num,inp[2]);
     char *name_every = strtok(name_p,"&&");
     while(name_every)
     {printf("<%s>\n",name_every);
      name_every = strtok(NULL,"&&");
     } 
  }

  close(cfd);
  return 0;
}
