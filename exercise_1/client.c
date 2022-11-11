#include <arpa/inet.h> /* htons, inet_addr */
#include <endian.h>
#include <netinet/in.h> /*inet_addr */
#include <stdio.h>      /* fprintf, perror */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset */
#include <sys/socket.h> /* connect, inet_addr, shutdown, socket */
#include <sys/types.h>  /* connect, inet_addr, read, socket, write */
#include <sys/uio.h>    /* read, write */
#include <unistd.h>     /* close, read, write */

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 12345

#define MSG_BUFFER_SIZE 1024

void stop_connection(int s) {
  /* Stop the connection */
  if (shutdown(s, SHUT_RDWR) == -1) {
    perror("shutdown");
    exit(1);
  }
}

void close_socket(int s) {
  /* Close the socket */
  if (close(s) == -1) {
    perror("close");
    exit(1);
  }
}

ssize_t recv_msg(int s, char buf[], int size) {
  ssize_t cc;
  cc = recv(s, buf, size, 0);
  printf("%s\n", buf);
  return cc;
}

int main(void) {
  int s, cc;
  struct sockaddr_in sa;
  char buf[MSG_BUFFER_SIZE];

  /* Create the socket */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket");
  }

  /* Set connection name */
  memset(&sa, 0, sizeof(sa));
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_family = AF_INET;
  inet_pton(AF_INET, SERVER_ADDR, &sa.sin_addr);

  /* Connect to server */
  fprintf(stderr, "Connecting to the server...\n");
  if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
    perror("connect");
    close_socket(s);
    return 1;
  }
  fprintf(stderr, "Connected.\n");

  /* Receive message from the server and show it on the screen */
  fprintf(stderr, "Message from the server:\n\n");
  if ((cc = recv_msg(s, buf, sizeof(buf))) == -1) {
    perror("receive");
    stop_connection(s);
    close_socket(s);
    return 1;
  }
  fprintf(stderr, "\n\nFinished receiving.\n");

  /* Stop connection */
  stop_connection(s);

  /* Close the socket */
  close_socket(s);

  return 0;
}
