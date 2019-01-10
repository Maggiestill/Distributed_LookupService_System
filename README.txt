# Distributed_LookupService_System
Description: 3 backend servers, 1 client-facing server(AWS), 1 client and 1 monitor






1. For AWS: 
       The aws server first initialize all the sockets it need, build all connections, then waiting to receive message from the client. It opened two listen() for client and monitor.
       After receiving message from client, aws first saperate function and input(key word) from the message, show what it received. Then combine it again and send it to all servers. 
       After it receives all the udp sockets including result found from ever database from the servers, it will saperate the message again. In the message send from servers, I use some special symble such as "::" or "&&" to saperate each words get from database. So then aws can cut the words and number out from the message. 
      Aws dealing with all the words then combined it together and send to client and monitor.
      For the monitor, aws also need to send the input and fuction type since monitor know nothing about the fuction type and key words.

2. For backend serverA B C:
       All the three servers do initialize first, then build connection using udp with aws.
       Server also need to open database.
       Then aws will send socket to server, the message contains the keyword and function. After saperating it, server search key words from the database one line by another line and count the match number. Here used strcmp and strncmp.
       After getting all the result, server combine it together and send to aws using udp.
   
3. For client:
       Input fuction and key words, client first judge whether the input format is correct. After checking and getting the fuction type, client combine it together then send to aws.
       After getting the tcp socket from aws, client cut it into words accoring to the fuction and display the final result.

4. For monitor:
       Monitor and client running on the different port so in aws, initialize and connetcing tcp is operated twice.
       Then Monitor receive the socket from aws, containing result. Monitor cut it into several pieces, get match number, fuction type, the keys word, and also the search result. Then display on termial.

5. I didn't change format from the required format.
   "prefix" for prefix, "search" for search.
   For all the search, it is case sensitive.

6. For the initialiaze and setting up sockets, I used some code from: 
       Beej's Guide
       http://blog.csdn.net/shine_journey/article/details/44037285
       http://bbs.csdn.net/topics/390476678
   For comparing words, I used some code from:
       https://blog.csdn.net/jianghong678/article/details/47152907
   



