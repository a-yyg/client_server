/* socket/server.c */
#include <arpa/inet.h>  /* htons */
#include <stdio.h>      /* fprintf, perror */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset, strlen */
#include <sys/socket.h> /* accept, bind, listen, setsockopt, shutdownm socket */
#include <sys/types.h>  /* accept, bind, setsockopt, socket, write */
#include <sys/uio.h>    /* write */
#include <unistd.h>     /* close, write */

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 12345

enum {
  NQUEUESIZE = 5,
};

char *message = "Hello!\nGood-bye!!\n";

int main(void) {
  int s, ws, soval, cc;
  struct sockaddr_in sa, ca;
  socklen_t ca_len;

  /* Create socket */
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Set options for reusing address */
  soval = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  /* Set name to the socket */
  memset(&sa, 0, sizeof(sa));
  // sa.sin_len = sizeof(sa);    /* sin_len がある場合 */
  sa.sin_family = AF_INET;
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    perror("bind");
    exit(1);
  }

  /* Set up queue */
  if (listen(s, NQUEUESIZE)) {
    perror("listen");
    exit(1);
  }

  fprintf(stderr, "Ready.\n");

  for (;;) {
    /* Accept connection */
    fprintf(stderr, "Waiting for a connection...\n");
    ca_len = sizeof(ca);
    if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
      perror("accept");
      exit(1);
    }
    fprintf(stderr, "Connection established.\n");

    /* Send message to the client */
    fprintf(stderr, "Sending the message...\n");
    if ((cc = write(ws, message, strlen(message))) == -1) {
      perror("shutdown");
      exit(1);
    }

    /* Stop the connection */
    if (shutdown(ws, SHUT_RDWR) == -1) {
      perror("shutdown");
      exit(1);
    }

    /* Close the socket */
    if (close(ws) == -1) {
      perror("close");
      exit(1);
    }
    /* On to the next request */
  }
  /* Infinite loop */
}
