#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_STRINGS 100

int main() {
  int pipefd[2];
  char strings[MAX_STRINGS][100];
  int num_strings;
  int i;

  // Create a pipe.
  pipe(pipefd);

  // Fork a child process.
  pid_t pid = fork();

  if (pid == 0) {
    // Child process.

    // Close the write end of the pipe.
    close(pipefd[1]);

    // Read strings from the pipe.
    for (i = 0; i < num_strings; i++) {
      read(pipefd[0], strings[i], 100);
    }

    // Close the read end of the pipe.
    close(pipefd[0]);

    // Check each string for palindromes.
    for (i = 0; i < num_strings; i++) {
      int j = 0;
      int k = strlen(strings[i]) - 1;
      while (j < k && strings[i][j] == strings[i][k]) {
        j++;
        k--;
      }

      if (j >= k) {
        // The string is a palindrome.
        printf("%s is a palindrome.\n", strings[i]);
      }
    }

    // Exit the child process.
    exit(0);
  } else {
    // Parent process.

    // Close the read end of the pipe.
    close(pipefd[0]);

    // Write the number of strings to the pipe.
    write(pipefd[1], &num_strings, sizeof(int));

    // Write the strings to the pipe.
    for (i = 0; i < num_strings; i++) {
      write(pipefd[1], strings[i], 100);
    }

    // Close the write end of the pipe.
    close(pipefd[1]);

    // Wait for the child process to finish.
    wait(NULL);
  }

  return 0;
}
