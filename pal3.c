#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_STRINGS 100
#define MAX_STRING_LENGTH 100

int main() {
    int pipefd[2];
    char strings[MAX_STRINGS][MAX_STRING_LENGTH];
    char palindromes[MAX_STRINGS][MAX_STRING_LENGTH]; // To store palindromes
    int num_strings;
    int num_palindromes = 0; // Counter for palindromes
    int i;

    // Create a pipe.
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    printf("Enter the number of strings: ");
    scanf("%d", &num_strings);

    // Parent process.
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process.

        // Close the write end of the pipe.
        close(pipefd[1]);

        // Read strings from the pipe.
        for (i = 0; i < num_strings; i++) {
            read(pipefd[0], strings[i], MAX_STRING_LENGTH);
        }

        // Close the read end of the pipe.
        close(pipefd[0]);

        // Check each string for palindromes.
        for (i = 0; i < num_strings; i++) {
            int j = 0;
            int k = strlen(strings[i]) - 1;
            int is_palindrome = 1; // Flag to indicate if current string is palindrome

            while (j < k) {
                if (strings[i][j] != strings[i][k]) {
                    is_palindrome = 0; // Not a palindrome
                    break;
                }
                j++;
                k--;
            }

            if (is_palindrome) {
                // If current string is palindrome, store it in palindromes array
                strcpy(palindromes[num_palindromes], strings[i]);
                num_palindromes++; // Increment counter
            }
        }

        // Write palindromes to the pipe.
        for (i = 0; i < num_palindromes; i++) {
            write(pipefd[1], palindromes[i], MAX_STRING_LENGTH);
        }

        // Close the write end of the pipe.
        close(pipefd[1]);

        // Exit the child process.
        exit(EXIT_SUCCESS);
    } else {
        // Parent process.

        // Close the read end of the pipe.
        close(pipefd[0]);

        // Write the strings to the pipe.
        printf("Enter %d strings:\n", num_strings);
        for (i = 0; i < num_strings; i++) {
            scanf("%s", strings[i]);
            write(pipefd[1], strings[i], MAX_STRING_LENGTH);
        }

        // Close the write end of the pipe.
        close(pipefd[1]);

        // Read and display the palindromes from the pipe.
        printf("Palindromes received from child process:\n");
        for (i = 0; i < num_strings; i++) {
            read(pipefd[0], strings[i], MAX_STRING_LENGTH);
            printf("%s\n", strings[i]);
        }

        // Close the read end of the pipe.
        close(pipefd[0]);

        // Wait for the child process to finish.
        wait(NULL);
    }

    return 0;
}
