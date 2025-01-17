#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MIN_PASSWORD_LENGTH 8 // Define a minimum acceptable password length

// Function to check if the password meets the requirements
int isValidPassword(const char *password) {
    int hasUpper = 0;
    int hasPunct = 0;

    // Check each character in the password
    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) hasUpper = 1;
        if (ispunct(password[i])) hasPunct = 1;
    }

    // Return true if both conditions are met
    return hasUpper && hasPunct;
}

// Function to generate a random password
void generatePassword(char *password, int length, const char *charset, int charsetLength) {
    for (int i = 0; i < length; i++) {
        password[i] = charset[rand() % charsetLength];
    }
    password[length] = '\0'; // Null-terminate the password string
}

// Function to parse the command-line argument for password length
int parseLengthArgument(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--length=", 9) == 0) {
            return atoi(argv[i] + 9); // Extract the length value after '='
        }
    }
    return -1; // Return -1 if no valid argument is found
}

int main(int argc, char *argv[]) {
    int length;
    char *password;

    // Define the character set for generating passwords
    const char charset[] = "abcdefghijklmnopqrstuvwxyz"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "0123456789"
                           "!@#$%^&*()-_=+";
    const int charsetLength = strlen(charset);

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Check if the user provided a command-line argument for length
    length = parseLengthArgument(argc, argv);

    if (length == -1) {
        // If no valid argument was provided, prompt the user for input
        printf("Enter desired password length (minimum %d): ", MIN_PASSWORD_LENGTH);
        if (scanf("%d", &length) != 1 || length < MIN_PASSWORD_LENGTH) {
            fprintf(stderr, "Invalid input. Password length must be at least %d.\n", MIN_PASSWORD_LENGTH);
            return EXIT_FAILURE;
        }
    } else if (length < MIN_PASSWORD_LENGTH) {
        // Handle case where the provided length is below the minimum
        fprintf(stderr, "Error: Password length must be at least %d.\n", MIN_PASSWORD_LENGTH);
        return EXIT_FAILURE;
    }

    // Allocate memory for the password
    password = (char *)malloc(length + 1);
    if (password == NULL) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    // Generate passwords until a valid one is found
    do {
        generatePassword(password, length, charset, charsetLength);
    } while (!isValidPassword(password));

    // Display the generated password
    printf("Generated password: %s\n", password);

    // Free allocated memory
    free(password);

    return EXIT_SUCCESS;
}
