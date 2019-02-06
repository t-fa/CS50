#include <cs50.h>
#include <ctype.h> //isalpha
#include <stdio.h>
#include <stdlib.h> //atoi
#include <string.h> //strlen

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Error! Specify key in command line argument.\n");
        return 1;
    }
    else
    {
        string key = argv[1];
        int k = atoi(key);
        string plaintext = get_string("plaintext:  ");
        printf("ciphertext: ");
        for(int i=0, n = strlen(plaintext); i < n; i++)
        {
        if (isalpha(plaintext[i]))
            {
                if (isupper(plaintext[i]))
                {
                    printf("%c", (((plaintext[i] - 65) + k)%26 + 65));
                }
                if (islower(plaintext[i]))
                {
                    printf("%c", (((plaintext[i] - 97) + k)%26 + 97));
                }
            }
        else
            {
                printf("%c", plaintext[i]);
            }
        }
        printf("\n");
    }
}
