#include <cs50.h>
#include <stdio.h>
// This is sort of a combination of the less comfortable and more comfortable solutions
int main(void)
{
    // Get user input for height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 0 | height > 23); // Must be between 0 and 23

    for (int i = 0; i < height; i++) // Repeats inside loop or determines height, simply put
    {
        for (int j = 0; j < height-i-1; j++) // Makes number of spaces per row, decrementing by 1 each time.
        {
            printf(" ");
        }
        for (int k = 0; k < i+2; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int k = 0; k < i+2; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
