#include <stdio.h>
#include <cs50.h>

// This is where I initiate my functions. This is so they can be at the bottom
int get_posint_1to8(void); // Recieves an integer(output)
void pyramid(int n); // Integer as an input

int main(void)
{
    int n = get_posint_1to8(); // Calls function to check the number is between 1 and 8
    pyramid(n); // Recieves n and calls function that makes the pyramid
}


// Functions

int get_posint_1to8(void) // Function to check the number is between 1 and 8
{
    int n; // Initialize the variable n
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);
    return n;
}

void pyramid(int n) // Function that makes the pyramid
{
    for (int i = 0; i < n; i++)
    {
        // code to make a pyramid
        for (int k = 1; k < n - i; k++)
        {
            printf(" ");
        }
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        printf("  ");
        for (int m = 0; m <= i; m++)
        {
            printf("#");
        }
        printf("\n");
    }
}