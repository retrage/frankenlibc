/* import from: https://github.com/mafintosh/echo-servers.c/blob/master/tcp-echo-server.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Usage: %s [port]\n", argv[0]);
    return 1;
  }

  int port = atoi(argv[1]);

  int server_fd, client_fd, err;
  struct sockaddr_in server, client;
  char buf[BUFFER_SIZE];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    printf("Could not create socket\n");
    return 1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

  err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
  if (err < 0) {
    printf("Could not bind socket\n");
    return 1;
  }

  err = listen(server_fd, 128);
  if (err < 0) {
    printf("Could not listen on socket\n");
    return 1;
  }

  printf("Server is listen on %d\n", port);

  while (1) {
    socklen_t client_len = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);

    if (client_fd < 0) {
      printf("Could not establish new connection\n");
      return 1;
    }

    while (1) {
      int read = recv(client_fd, buf, BUFFER_SIZE, 0);

      if (!read) break;
      if (read < 0)  {
        printf("Client read failed\n");
        return 1;
      }

      printf("recv: %s\n", buf);

      err = send(client_fd, buf, read, 0);
      if (err < 0) {
        printf("Client write failed\n");
        return 1;
      }
    }
  }

  return 0;
}
