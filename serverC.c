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

#define MAXchar 1000  /*for reading the database txt document*/





int main()

{

  struct sockaddr_in server_addr,client_addr;

  char buf[1024];

  char bufrs[1024];               // buffer of result of SEARCH

  char bufrp[1024];               // buffer of result of PREFIX

  int server_sockfd;



 /*read file and save to str*/

  FILE *fp;

  char *filename = "/home/ee450/Desktop/ee450_yiranma@usc.edu_session2/backendC.txt";



  /*initialize*/

  memset(&server_addr,0,sizeof(server_addr));

  server_addr.sin_family=AF_INET;

  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

  server_addr.sin_port=htons(PORT_SERVER_C);

  memset(&client_addr,0,sizeof(client_addr));

  client_addr.sin_family=AF_INET;

  client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

  client_addr.sin_port=htons(PORT_AWS_UDP);



  /*create socket*/

  if((server_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)

  {

    perror("socket error:");

    return 1;

  }



  /*bind socket*/

  if(bind(server_sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))<0)

  {

    perror("bind error:");

    return 1;

  }

  printf("The Server C is up and running using UDP on port <%d>\n",PORT_SERVER_C);



  while(1)

  {

    int sin_size = sizeof(struct sockaddr_in);

    if(recvfrom(server_sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&sin_size)<0)

    {

      perror("recvfrom error:");

      return 1;

    }



    /*saperate function and value from the received message, save into two char*/

    int i,j=0,k=0;

    char function[1024],input[1024];

    for(i=0;i<6;i++)

    {function[j++]=buf[i];}

    for(i=7;i<sizeof(buf);i++)

    {input[k++]=buf[i];}

    printf("The server C received input <%s> and operation <%s>\n",input,function);





    /***SEARCH function***/

    if(strcmp(function,"search") == 0)

    { 

      char name_s[1000]="";

       char str_s[MAXchar];          /*for reading the database, save everyline to str_p*/

       int  manums=0;

       fp = fopen(filename, "r");    /*open database C*/

       if(fp == NULL)                  

       {   printf("Could not open file %s\n", filename);

           return 1;}

 

       while(fgets(str_s, MAXchar, fp)!=NULL)

       {

           char *everyname_s= strtok(str_s,"::");

           char *everydefination_s = strtok(NULL,"::");

           if(strcmp(input,everyname_s)==0)      /*use strncmp fuction for predix compare*/

           {

             sprintf(everyname_s,"%s%s", everyname_s,"&&"); 

             sprintf(name_s,"%s%s", name_s,everyname_s);

             manums=manums+1;

           } 

       }

       printf("Server C has found <%d> matches\n",manums);

       fclose(fp); 



       char result_s[1000];

       char macs[20];

       sprintf(macs,"%d", manums);

       sprintf(result_s,"%s%s%s", macs,"::",name_s);      /*number and words combined to one char and send*/

       if(sendto(server_sockfd,result_s,sizeof(result_s),0,(struct sockaddr*)&client_addr,sizeof(client_addr))<0)

       {

        perror("sendto error:");

        return 1;

       }

    //printf("the combining result of A\n<%s>\n",result_p);

    printf("The Server C finished sending the output to AWS\n");

    }



    /***PREFIX function***/

    else

    {

       char name_p[1000]="";

       char str_p[MAXchar];          /*for reading the database, save everyline to str_p*/

       int manum=0;

       fp = fopen(filename, "r");    /*open database A*/

       if(fp == NULL)                  

       {   printf("Could not open file %s\n", filename);

           return 1;}

 

       while(fgets(str_p, MAXchar, fp)!=NULL)

       {

           char *everyname_p= strtok(str_p,"::");

           char *everydefination_p = strtok(NULL,"::");

           if(strncmp(input,everyname_p,strlen(input))==0)      /*use strncmp fuction for predix compare*/

           {

             sprintf(everyname_p,"%s%s", everyname_p,"&&"); 

             sprintf(name_p,"%s%s", name_p,everyname_p);

             manum=manum+1;

           } 

       }

       printf("Server C has found <%d> matches\n",manum);

       fclose(fp); 



       char result_p[1000];

       char mac[20];

       sprintf(mac,"%d", manum);

       sprintf(result_p,"%s%s%s", mac,"::",name_p);      /*number and words combined to one char and send*/

       if(sendto(server_sockfd,result_p,sizeof(result_p),0,(struct sockaddr*)&client_addr,sizeof(client_addr))<0)

       {

        perror("sendto error:");

        return 1;

       }

    //printf("the combinding result of C\n<%s>\n",result_p);

    printf("The Server C finished sending the output to AWS\n");

    }

   

  }

  close(server_sockfd);

  return 0;

}
