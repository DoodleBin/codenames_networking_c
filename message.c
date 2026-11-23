#include "message.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

// Send a across a socket with a header that includes the message length.
int send_message(int fd, const char* message) {
  // If the message is NULL, set errno to EINVAL and return an error
  if (message == NULL) {
    errno = EINVAL;
    return -1;
  }

  // First, send the length of the message in a size_t
  size_t len = strlen(message);
  // Passing MSG_NOSIGNAL asks the OS not to send SIGPIPE when the socket is closed.
  // If we receive that signal and don't have a handler set, the program will exit immediately.
  if (send(fd, &len, sizeof(size_t), MSG_NOSIGNAL) != sizeof(size_t)) {
    // Writing failed, so return an error
    return -1;
  }

  // Now we can send the message. Loop until the entire message has been written.
  size_t bytes_written = 0;
  while (bytes_written < len) {
    // Try to write the entire remaining message, using MSG_NOSIGNAL here as well
    ssize_t rc = send(fd, message + bytes_written, len - bytes_written, MSG_NOSIGNAL);

    // Did the write fail? If so, return an error
    if (rc <= 0) return -1;

    // If there was no error, write returned the number of bytes written
    bytes_written += rc;
  }

  return 0;
}

// Receive a message from a socket and return the message string (which must be freed later)
char* receive_message(int fd) {
  // First try to read in the message length
  size_t len;
  if (recv(fd, &len, sizeof(size_t), 0) != sizeof(size_t)) {
    // Reading failed. Return an error
    return NULL;
  }

  // Now make sure the message length is reasonable
  if (len > MAX_MESSAGE_LENGTH) {
    errno = EINVAL;
    return NULL;
  }

  // Allocate space for the message and a null terminator
  char* result = malloc(len + 1);

  // Try to read the message. Loop until the entire message has been read.
  size_t bytes_read = 0;
  while (bytes_read < len) {
    // Try to read the entire remaining message
    ssize_t rc = recv(fd, result + bytes_read, len - bytes_read, 0);

    // Did the read fail? If so, return an error
    if (rc <= 0) {
      free(result);
      return NULL;
    }

    // Update the number of bytes read
    bytes_read += rc;
  }

  // Add a null terminator to the message
  result[len] = '\0';

  return result;
}
