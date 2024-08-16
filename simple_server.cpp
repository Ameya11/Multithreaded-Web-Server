/* run using ./server <port> */
#include "http_server.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <queue>

#include <pthread.h>
#define buffsize 4096

queue<int> my_queue;

using namespace std;
pthread_mutex_t count_mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_variable = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_variable  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queue_lock_variable = PTHREAD_MUTEX_INITIALIZER;


// Print the queue
void showq(queue<int> gq)
{
    queue<int> g = gq;
    while (!g.empty()) {
        cout << '\t' << g.front();
        g.pop();
    }
    cout << '\n';
}



void error(char *msg) {
  perror(msg);
  exit(1);
}




void *start_function(void *arg) {
while(1){
     pthread_mutex_lock( &lock_variable );
    //cout<< "\n I'm a thread who's in the sleeping state"<< endl;
    pthread_cond_wait( &cond_variable, &lock_variable );
    //cout<< "\n I'm awake give me some work \n";
      pthread_mutex_unlock( &lock_variable);



      int fd_to_serve;
      pthread_mutex_lock( &queue_lock_variable);
      fd_to_serve = my_queue.front();
      my_queue.pop();
      //cout<< " A new fd is being serviced \n";
      pthread_mutex_unlock( &queue_lock_variable);




   int newsockfd = fd_to_serve;
   char buffer[buffsize];

   int n;
   // ...thread processing...

     /* read message from client */

     bzero(buffer, buffsize);
     n = read(newsockfd, buffer, buffsize-1);
     if (n < 0)
       error("ERROR reading from socket");
     // printf("The message received is: %s", buffer);

     /* send reply to client */
     string bufferstr(buffer);
     HTTP_Response *a = handle_request(bufferstr);
     string resp = a->get_string();
     //cout<< resp;
     cout << "\n----------------------------------------------------\n";
     cout << " *--Response Received--* with status code: "<< a->status_code;
     cout << "\n----------------------------------------------------\n";

     n = write(newsockfd, resp.c_str(), resp.length());
     if (n < 0)
       error("ERROR writing to socket");
close(newsockfd);
}
 }

int main(int argc, char *argv[]) {


  	pthread_t tid[10];
    int thread_arg1 = 0;
  for(int i=0;i<10;i++){
  	pthread_create(&tid[i], NULL, start_function, &thread_arg1);
    // sleep(1);
  }
  sleep(1);










  int sockfd, newsockfd, portno;
  socklen_t clilen;



  char buffer[buffsize];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  /* create socket */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind socket to this port number on this machine */


  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* listen for incoming connection requests */

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  /* accept a new request, create a newsockfd */
while(1){
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
   cout << "\n----------------------------------------------------\n";
  cout << " [+]--New connection accepted--[+]";
   cout << "\n----------------------------------------------------\n";
  if (newsockfd < 0)
    error("ERROR on accept");
  int thread_arg = newsockfd;
  pthread_t thread_id;


pthread_mutex_lock( &queue_lock_variable);
my_queue.push(newsockfd);
pthread_mutex_unlock( &queue_lock_variable);

pthread_mutex_lock( &lock_variable);
pthread_cond_signal( &cond_variable);
pthread_mutex_unlock( &lock_variable);

   // pthread_create(&thread_id, NULL, start_function, &thread_arg);
}
  return 0;
}
