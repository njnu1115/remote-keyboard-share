#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int event_fd = -1;

void send_keyboard_event(int fd, unsigned short type, unsigned short code,
                         int value) {
  struct input_event event;
  memset(&event, 0, sizeof(event));

  gettimeofday(&event.time, NULL);
  event.type = type;
  event.code = code;
  event.value = value;

  if (write(fd, &event, sizeof(event)) < 0) {
    perror("Failed to write to /dev/input/event3");
    exit(EXIT_FAILURE);
  }
}

void receive_message(int port) {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  char buffer[BUFFER_SIZE];

  // Create a socket
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Bind the socket to the port
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Binding failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_socket, 5) == -1) {
    perror("Listening failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Waiting for incoming connection...\n");

  while (1) {
    // Accept a connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                                &client_addr_len)) == -1) {
      perror("Accepting connection failed");
      close(server_socket);
      exit(EXIT_FAILURE);
    }

    printf("Connected by %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    // Receive data from the client
    ssize_t num_bytes_received;
    while ((num_bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) >
           0) {
      buffer[num_bytes_received] = '\0'; // Null-terminate the received data
      printf("Received %ld bytes: %s\n", num_bytes_received, buffer);
      send_keyboard_event(event_fd, EV_KEY, buffer[1], buffer[0]);
      send_keyboard_event(event_fd, EV_SYN, SYN_REPORT, 0);
    }

    // Close client socket
    printf("Client closed the connection\n");
    close(client_socket);
  }

  // Close server socket (not reached in this code)
  close(server_socket);
}

int main() {
  int port = 38104; // Change this to the port you want to listen on
  event_fd = open("/dev/input/event3", O_WRONLY);
  if (event_fd == -1) {
    perror("Failed to open /dev/input/event3");
    return 1;
  }

  receive_message(port);

  return 0;
}
