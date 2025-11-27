#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdbool.h>
#include <poll.h>
#define BACKLOG 10
#define TIMEOUT 2000
#define NUM_CLIENTS 10

int insert(int sockfd, struct pollfd *sockets){

  for(int i = 1; i < NUM_CLIENTS + 1; i++){
    if (sockets[i].fd == -1){
      sockets[i].fd = sockfd;
      sockets[i].events = POLLIN;
      return i;
    }
  }

  return -1;
}

//chatgpt citation - timer code
double current_time_secs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}


int main( int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: rebel-fleet port\n");
    return 1;
  }

  int port = atoi(argv[1]);
  printf("Binding to port %d...\n", port);

  /* Create IPv4 stream socket */
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  /* Create the sockaddr */
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port); 
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Add code here to bind, listen, accept connections */
  bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  int numfd = 1;
  char data[1000000];
  listen(sockfd, BACKLOG);
  struct pollfd sockets[NUM_CLIENTS + 1];
  sockets[0].fd = sockfd;
  sockets[0].events = POLLIN;

  for(int i = 1; i < NUM_CLIENTS + 1; i++ ){
    sockets[i].fd = -1;
  }
  int numfds = 1;
  int count = 0;
  double last_activity = current_time_secs();

  while(1){
    int num_events = poll(sockets, numfds, TIMEOUT);
    double now = current_time_secs();
    
    //CHATGPT CITATION: Extremely cool technique to listen to new connections on the listening socket
    // and to recv data from the clients as well
    //only the array structure and outer if conditions were used from chatgpt
    for(int i = 0; i < numfds; i++){
      if(i == 0){
        if ((sockets[i].revents & POLLIN) && numfds <= NUM_CLIENTS + 1){
        int newsockfd = accept(sockfd, NULL, NULL);
        int index = insert(newsockfd, sockets);
      if(index == numfds){
        numfds++;
      }
        last_activity = now;
        }
      }

      else{
        if(sockets[i].revents & POLLIN){
          ssize_t bytes = recv(sockets[i].fd, data, 1000000, 0);
          if(bytes == 0){
            close(sockets[i].fd);
            sockets[i].fd = -1;
            sockets[i].events = 0;
            if(i == numfds - 1){
            numfds--;
            }
          }
          else{
          char file[100];
          sprintf(file, "data%d.dat", count + 1);
          count++;
          FILE* fp = fopen(file, "w");
          fwrite(data, 1, bytes, fp);
          fclose(fp);
          }
          last_activity = now;
        }
      }
    }

    if(now - last_activity >= 5){
      break;
    }
  }
    
  close(sockfd);
  return 0;
}

