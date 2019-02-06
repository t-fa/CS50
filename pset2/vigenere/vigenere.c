#include <cs50.h>
#include <ctype.h> //isalpha
#include <stdio.h>
#include <string.h> //strlen

int main(int argc, string argv[])
{
    if (argc != 2) // Ensures exactly 2 arguments in command line
    {
        printf("Error! Specify key in command line argument.\n");
        return 1; // Signifies error
    }
    for(int i=0, n = strlen(argv[1]); i < n; i++) // iterates over every char in argv[1] to make sure it's alpha
    {
        if(!isalpha(argv[1][i]))
        {
            printf("Error! Command line argument must be alphabetical.\n");
            return 1; // Signifies error
        }
    }
    string plaintext = get_string("plaintext:  "); // Get input from user as var "plaintext"
    printf("ciphertext: "); // iterates over plaintext and encrypts it
    int i=0;
    for(int j=0, m = strlen(plaintext); j < m; j++) // i corresponds to command line key. j corresponds to plaintext
        {
            int index = (i % strlen(argv[1])); // Gives us the index of the key, wrapped around
            char k = argv[1][index];
            if (isupper(k))
            {
                k -= 65;
            }
            else if (islower(k))
            {
                k -= 97;
            }
            if (isalpha(plaintext[j])) // Still need this to preserve spaces, grammar, etc.
            {
                if (isupper(plaintext[j]))
                {
                    printf("%c", (((plaintext[j] - 65) + k) % 26) + 65);
                    i++;
                }
                if (islower(plaintext[j]))
                {
                    printf("%c", (((plaintext[j] - 97) + k) % 26) + 97);
                    i++;
                }
            }
            else
            {
                printf("%c", plaintext[j]); // Print output for non-alpha chars.
            }
        }
        printf("\n");
}
