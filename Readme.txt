Group details : USN and names
PES University MTech 2018-2020

PES1201802353  ROOPESH R
PES1201802600  SADAQATH HUSSAIN.H
PES1201802455  RAJAT P RAYADURG

How the executable is created (just actual command which we used)
Compile the code of server and client (iterative server)
cc tcpserver.c -o tcpserver
cc tcpclient.c -o tcpclient


How to execute the output file (for iterative server)

./tcpserver
Enter port number for server to communicate
>9000
Waiting for client #1 client to connect ... 
Client with address  127.0.0.1 has connected
message received from the client is "hello from client"

./tcpclient
Enter the port for client to communicate
>9000
Enter the server address for client to communicate
>127.0.0.1
Enter message to be transmitted to the server
>hello from client
message received from server is "byebye"
Enter message to be transmitted to the server
>


Description of how the program works (5-6 lines) and all the error conditions that we looked at
and are handled in the program. 

>>for iterative server
The iterative server handles one client at a time,on a specific port. When multiple clients send requests to server, they are queued one after the other. When the client(whose request is being processed) quits,the next client from queue is served. Some error conditions like server exiting abruptly,client closing the connection have been handled using signal handling.

>> for concurrent server
The concurrent server on the other hand serves clients concurrently ass and when they request the server. There is an instance of the server dedicated to listening on the specified port to accept requests from clients. Once a client request is accepted ,another instance of the server will serve the client.
Error conditions like server quitting after accepting n connections,and clients quitting after connecting to server are handled by sending special charaters and signal handling.

cc tcpConcurrentServer.c -o tcpconser
./tcpconser
cc tcpclient.c -o tcpclient
./tcpclient


How to execute bonus question of sending random number(limited by an upper bound) of requests to server?
cc tcpclient_bonus_q.c -o bonus
./bonus

How to execute bonus question of generating 1000+ clients each sending random number of requests (limited by an upper bound)
cc gen1000cli.c -o gen1000cli
./gen1000cli 



Challenges/issues that we faced while writing such program and how did we address the
same.
Handling the error conditions was challenging


Details and explanations of result obtained
The client and server programs were subjected to following test :
1)Client sends random number of requests to the server

Observations:
RandomRequests	Time to send random request
29		2ms
42		3ms
49		4ms
171		10ms
178		9ms
191		11ms
236		11ms
316		14ms
1569		55ms
1664		58ms
4459		147ms

2)N (1000+) clients were  generated with random number of request being sent by every client to the server. 

time taken for generating 1000 clients (that request the server) that send random number of request was 1973 ms.
time taken for generating 1200 clients (that request the server) that send random number of request was 3848 ms (but several server instances quit abruptly).

Subsequently larger the number of clients generated, more the server instances that quit.
















