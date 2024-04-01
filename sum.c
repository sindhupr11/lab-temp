
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int pipefds[2];
  int n;
  int sum = 0;
  int i;

  // Create a pipe.
  if (pipe(pipefds) == -1) {
    perror("pipe");
    exit(1);
  }

  // Create a child process.
  int pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  }

  // Parent process.
  if (pid > 0) {
    // Close the read end of the pipe.
    close(pipefds[0]);

    // Read the number of numbers from the user.
    printf("Enter the number of numbers: ");
    scanf("%d", &n);

    // Send the number of numbers to the child process.
    write(pipefds[1], &n, sizeof(n));

    // Send the numbers to the child process.
    for (i = 0; i < n; i++) {
      int number;
      printf("Enter number %d: ", i + 1);
      scanf("%d", &number);
      write(pipefds[1], &number, sizeof(number));
    }

    // Close the write end of the pipe.
    close(pipefds[1]);

    // Wait for the child process to finish.
    wait(NULL);

    // Read the sum of the numbers from the child process.
    read(pipefds[0], &sum, sizeof(sum));

    // Display the sum of the numbers.
    printf("The sum of the numbers is %d\n", sum);
  }

  // Child process.
  else {
    // Close the write end of the pipe.
    close(pipefds[1]);

    // Read the number of numbers from the parent process.
    read(pipefds[0], &n, sizeof(n));

    // Read the numbers from the parent process.
    int numbers[n];
    for (i = 0; i < n; i++) {
      read(pipefds[0], &numbers[i], sizeof(numbers[i]));
    }

    // Calculate the sum of the numbers.
    for (i = 0; i < n; i++) {
      sum += numbers[i];
    }

    // Send the sum of the numbers to the parent process.
    write(pipefds[1], &sum, sizeof(sum));

    // Close the read end of the pipe.
    close(pipefds[0]);
  }

  return 0;
}
