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

int main()
{
  char buffer[1024];
  char buffer_a[1024];
  char buffer_b[1024];
  char buffer_c[1024];
  char numc[20];
  char numcs[20];
  char result[1024];
  char resultm[1024];

  /*initialization for TCPclient socket*/
  int sfd, sfdm, cfd, mfd, udp_sockfd;
  struct sockaddr_in s_add, c_add;
  memset(&s_add,0,sizeof(s_add));
  s_add.sin_family=AF_INET;
  s_add.sin_addr.s_addr=inet_addr("127.0.0.1");
  s_add.sin_port=htons(PORT_AWS_TCP_C);
  /*initialization for TCPmonitor socket*/
  struct sockaddr_in s2_add, m_add;
  memset(&s2_add,0,sizeof(s2_add));
  s2_add.sin_family=AF_INET;
  s2_add.sin_addr.s_addr=inet_addr("127.0.0.1");
  s2_add.sin_port=htons(PORT_AWS_TCP_M);


  /*initialization for UDP socket*/
  struct sockaddr_in AWS_addr_UDP, serverA_addr, serverB_addr, serverC_addr;
  memset(&AWS_addr_UDP,0,sizeof(AWS_addr_UDP));
  AWS_addr_UDP.sin_family=AF_INET;
  AWS_addr_UDP.sin_addr.s_addr=inet_addr("127.0.0.1");
  AWS_addr_UDP.sin_port=htons(PORT_AWS_UDP);

  memset(&serverA_addr,0,sizeof(serverA_addr));
  serverA_addr.sin_family=AF_INET;
  serverA_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  serverA_addr.sin_port=htons(PORT_SERVER_A);

  memset(&serverB_addr,0,sizeof(serverB_addr));
  serverB_addr.sin_family=AF_INET;
  serverB_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  serverB_addr.sin_port=htons(PORT_SERVER_B);

  memset(&serverC_addr,0,sizeof(serverC_addr));
  serverC_addr.sin_family=AF_INET;
  serverC_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  serverC_addr.sin_port=htons(PORT_SERVER_C);
 


  /*Create TCP socket for client*/
  if((sfd = socket(PF_INET, SOCK_STREAM, 0))<0)
  {
    perror("socket error:\n");
    return 1;
  }

  /*Create TCP socket for monitor*/
  if((sfdm = socket(PF_INET, SOCK_STREAM, 0))<0)
  {
    perror("socket error:\n");
    return 1;
  }


  if(bind(sfd,(struct sockaddr*)&s_add,sizeof(struct sockaddr))<0)/*bind for client*/
  {
    perror("bind error:\n");
    return 1;
  }

  if(bind(sfdm,(struct sockaddr*)&s2_add,sizeof(struct sockaddr))<0)/*bind for monitor*/
  {
    perror("bind error:\n");
    return 1;
  }


  if(listen(sfd,20)<0)  /*listen for client*/
  {
    perror("listen error:\n");
    return 1;
  }  
  
 
  if(listen(sfdm,20)<0) /*listen for monitor*/
  {
    perror("listen error:\n");
    return 1;
  }  


  /*create UDP socket */
  if((udp_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
  {
    perror("socket error:\n");
    return 1;
  }


  if(bind(udp_sockfd, (struct sockaddr*)&AWS_addr_UDP,sizeof(struct sockaddr))<0)     /*bind UDP socket*/
  {
     perror("bind error:\n");
     return 1;
  }

  printf("The AWS is up and running.\n");



  while(1)
  { 

    /*accept TCP from client*/
    int sin_size = sizeof(struct sockaddr_in);
    if((cfd = accept(sfd,(struct sockaddr*)(&c_add),&sin_size))<0)      
    {
      perror("accept error:\n");
      return 1;
    }

    /*accept TCP from monitor*/
    if((mfd= accept(sfdm,(struct sockaddr*)(&m_add),&sin_size))<0)      
    {
      perror("accept error:\n");
      return 1;
    }

    /*****************receive function and value from client via TCP then sent to server via UDP*******************/
    /*receive socket from client*/
    if(recv(cfd,buffer,1024,0)<0)
    {
      perror("recv error:\n");
      return 1;
    }

    /*saperate fuction and input*/
    int i,j=0,k=0;
    char function[1024],input[1024];
    for(i=0;i<6;i++)
    {function[j++]=buffer[i];}
    for(i=7;i<sizeof(buffer);i++)
    {input[k++]=buffer[i];}
    printf("The AWS received input = <%s> and function = <%s> from the client using TCP over port <%d>\n", input, function, PORT_AWS_TCP_C);

    /*use UDP send request to three servers*/
    /*A*/     
    if(sendto(udp_sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr))<0)
    {
      perror("sendto error:");
      return 1;
    }
    printf("The AWS sent <%s> and <%s> to Backend-Server A\n",input,function);  
    /*B*/
    if(sendto(udp_sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverB_addr,sizeof(struct sockaddr))<0)
    {
      perror("sendto error:");
      return 1;
    }
    printf("The AWS sent <%s> and <%s> to Backend-Server B\n",input,function);  
    /*C*/
    if(sendto(udp_sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverC_addr,sizeof(struct sockaddr))<0)
    {
      perror("sendto error:");
      return 1;
    }
    printf("The AWS sent <%s> and <%s> to Backend-Server C\n",input,function);  

   

    /***********************use UDP receive search or prefix result from three servers***********************************/
    /*A*/
    if(recvfrom(udp_sockfd,buffer_a,sizeof(buffer_a),0,(struct sockaddr*)&serverA_addr,&sin_size)<0)
    {
      perror("A recvfrom error:");
      return 1;
    }
    /////printf("The AWS received <%s> matches from Backend-Server A using UDP over port <%d>\n",buffer_a,PORT_AWS_UDP);
    /*B*/
    if(recvfrom(udp_sockfd,buffer_b,sizeof(buffer_b),0,(struct sockaddr*)&serverB_addr,&sin_size)<0)
    {
      perror("B recvfrom error:");
      return 1;
    }
    ////printf("The AWS received <%s> matches from Backend-Server <B> using UDP over port <%d>\n",buffer_b,PORT_AWS_UDP);
    /*C*/
    if(recvfrom(udp_sockfd,buffer_c,sizeof(buffer_c),0,(struct sockaddr*)&serverC_addr,&sin_size)<0)
    {
      perror("C recvfrom error:");
      return 1;
    }
    ////printf("The AWS received <%s> matches from Backend-Server <C> using UDP over port <%d>\n",buffer_c,PORT_AWS_UDP);
 

    /****************************************deal with the final result*********************************************/
    /*deal with result for search*/
    if(strcmp(function,"search")==0) 
    {
       char *num1_s= strtok(buffer_a,"::");
       char *name1_s = strtok(NULL,"::");
       char *num2_s= strtok(buffer_b,"::");
       char *name2_s = strtok(NULL,"::");
       char *num3_s= strtok(buffer_c,"::");
       char *name3_s = strtok(NULL,"::");
       printf("The AWS received <%s> matches from Backend-Server A using UDP over port <%d>\n",num1_s,PORT_AWS_UDP);
       printf("The AWS received <%s> matches from Backend-Server B using UDP over port <%d>\n",num2_s,PORT_AWS_UDP);
       printf("The AWS received <%s> matches from Backend-Server C using UDP over port <%d>\n",num3_s,PORT_AWS_UDP);
       int intnum1_s = atoi(num1_s);
       int intnum2_s = atoi(num2_s);
       int intnum3_s = atoi(num3_s);      
       int nums=intnum1_s+intnum2_s+intnum3_s;   /*total number of matches in prefix*/
       printf("total number is : <%d>\n",nums);
       //printf("everyname is : <%s>\n<%s>\n<%s>\n",name1_p,name2_p,name3_p);
       char mresults[1000];
       sprintf(mresults,"%s%s%s",name1_s,name2_s,name3_s);  /*connet all match names into one char*/
       printf("The AWS sent <%s> result\n",mresults);
       sprintf(numc,"%d", nums);
       sprintf(result,"%s%s%s",numc,"::",mresults);  /*connet number and all match names into one char*/
       //printf("The AWS sent <%s> result\n",result);
       sprintf(resultm,"%s%s%s",input,"@@",result);

    }


    /*deal with result for prefix*/
    else 
    {
       char *num1_p= strtok(buffer_a,"::");
       char *name1_p = strtok(NULL,"::");
       char *num2_p= strtok(buffer_b,"::");
       char *name2_p = strtok(NULL,"::");
       char *num3_p= strtok(buffer_c,"::");
       char *name3_p = strtok(NULL,"::");
       printf("The AWS received <%s> matches from Backend-Server A using UDP over port <%d>\n",num1_p,PORT_AWS_UDP);
       printf("The AWS received <%s> matches from Backend-Server B using UDP over port <%d>\n",num2_p,PORT_AWS_UDP);
       printf("The AWS received <%s> matches from Backend-Server C using UDP over port <%d>\n",num3_p,PORT_AWS_UDP);

       int intnum1_p = atoi(num1_p);
       int intnum2_p = atoi(num2_p);
       int intnum3_p = atoi(num3_p);      
       int num=intnum1_p+intnum2_p+intnum3_p;   /*total number of matches in prefix*/
       //printf("total number is : <%d>\n",num);
       //printf("everyname is : <%s>\n<%s>\n<%s>\n",name1_p,name2_p,name3_p);
       char mresult[1000];
       sprintf(mresult,"%s%s%s",name1_p,name2_p,name3_p);  /*connet all match names into one char*/
       //printf("The AWS sent <%d> result\n",num);
       sprintf(numc,"%d", num);
       sprintf(result,"%s%s%s",numc,"::",mresult);  /*connet number and all match names into one char*/
       //printf("The AWS sent <%s> result\n",result);
       sprintf(resultm,"%s%s%s",input,"@@",result);

    }

    if(send(cfd,&result,sizeof(result),0)<0)  /*send final result to client via TCP*/
         {
           perror("server to client send error:\n");
           return 1;
         }
    printf("The AWS sent <%s> matches to client\n",numc);
    close(cfd);

    if(send(mfd,&resultm,sizeof(resultm),0)<0)  /*send final result to monitor via TCP*/ 
         {
           perror("server to monitor send error:\n");
           return 1;
         }
    printf("The AWS sent <%s> matches to monitor over port <%d>\n",numc,PORT_AWS_TCP_M);
    close(mfd);


  }
  
  close(udp_sockfd);
  close(sfd);
  close(sfdm); 
  return 0;
}
