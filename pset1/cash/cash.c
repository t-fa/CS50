#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);

    int coins = 0;
    int c = round(change*100);
    while (c > 0)
    {
        if (c > 24) // Quarters
        {
            c -= 25;
            coins+= 1;
        }
        if (c < 25 && c > 9) // Dimes
        {
            c -= 10;
            coins+= 1;
        }
        if (c < 10 && c > 4) // Nickels
        {
            c -= 5;
            coins+= 1;
        }
        if (c < 5 && c > 0) // Pennies
        {
            c -= 1;
            coins+= 1;
        }
    }
    printf("Coins: %i \n", coins);
}
