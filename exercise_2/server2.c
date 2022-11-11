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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MSG_BUFFER_SIZE 1024

enum {
  NQUEUESIZE = 5,
};

const char *disconnect_str = "\1";

void disconnect(int ws) { write(ws, disconnect_str, strlen(disconnect_str)); }

int main(void) {
  int s, ws, soval, cc;
  struct sockaddr_in sa, ca;
  socklen_t ca_len;
  char message[MSG_BUFFER_SIZE];

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

  fprintf(stderr, "[ready]\n");

  /* Accept connection */
  fprintf(stderr, "[waiting for a connection...]\n");
  ca_len = sizeof(ca);
  if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
    perror("accept");
    exit(1);
  }
  fprintf(stderr, "[connection established:\n");

  while (1) {
    if (feof(stdin)) {
      disconnect(ws);
      break;
    }

    {
      /* Get client information. */
      char ip_name[32];
      inet_ntop(AF_INET, &sa.sin_addr, ip_name, sizeof(ip_name));
      /* Print client information */
      fprintf(stdout, "\t%s : %d connected]\n", ip_name, sa.sin_port);
    }

    /* Wait for user input */
    memset(message, 0, sizeof(message));
    fgets(message, sizeof(message), stdin);
    int len = strlen(message);
    message[len - 1] = '\0';
    char new_msg[MSG_BUFFER_SIZE];
    new_msg[0] = '\2';
    strncpy(new_msg + 1, message, MIN(len, MSG_BUFFER_SIZE - 1));
    /* Send message to the client */
    fprintf(stderr, "[sending the message: %s]\n", new_msg);
    if ((cc = write(ws, new_msg, len)) == -1) {
      perror("shutdown");
      exit(1);
    }
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
}
