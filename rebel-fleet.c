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

    
  close(sockfd);
  return 0;
}

