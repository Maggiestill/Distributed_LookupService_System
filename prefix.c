#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXchar 1000  /*for reading the database txt document*/

#define PORT_SERVER_A 21475
#define PORT_AWS_UDP 24475


#define search search
#define prefix prefix



int main()
{

  /*read file and save to str*/
  FILE *fp;
  char *filename = "/home/ee450/Desktop/backendA.txt";

  char function[1024] = "prefix";
  char input[1024] = "Gene";
  printf("The server A received input <%s> and operation <%s>\n",input,function);


    /***SEARCH function***/
    if(strcmp(function,"search") == 0)
    {
    printf("The Server A finished sending the output to AWS\n");
    }
    



    /***PREFIX function***/
    else
    {
       char name_p[1000]="&&";
       char str_p[MAXchar];          /*for reading the database, save everyline to str_p*/
       int manum=0;
       fp = fopen(filename, "r");    /*open database A*/
       if(fp == NULL)                  
       {   printf("Could not open file %s", filename);
           return 1;}
 
       while(fgets(str_p, MAXchar, fp)!=NULL)
       {
           char *everyname_p= strtok(str_p,"::");
           printf("evername is <%s>\n",everyname_p);
           char *everydefination_p = strtok(NULL,"::");
           if(strncmp(input,everyname_p,strlen(input))==0)      /*use strncmp fuction for predix compare*/
           {
             sprintf(everyname_p,"%s%s", everyname_p,"&&"); 
             sprintf(name_p,"%s%s", name_p,everyname_p);
             manum=manum+1;
           } 
       }
       printf("Server A has found <%d> matches",manum);
       fclose(fp); 

       char result_p[1000];
       char mac[20];
       sprintf(mac,"%d", manum);
       sprintf(result_p,"%s%s%s", mac,"::",name_p);      /*number and words combined to one char and send*/
       
    printf("the binding result of A\n<%s>",result_p);
    printf("The Server A finished sending the output to AWS\n");
    }
   
  return 0;
}
