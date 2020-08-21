#include <stdio.h>
#include <cs50.h>

// This is where I initiate my functions. This is so they can be at the bottom
int cardlength(long n);
int checksum(long n, int len);
void checkcardtype(long n, int check, int len);

int main(void)
{
    long n = get_long("n: "); // Asks for the credit card number
    int len = cardlength(n); // Length of a card
    int check = checksum(n, len); // Checks with the Luhn's Algorithm. Returns the first two digits of the card
    if (check != 0) // If the past function returns a 0 instead of the two first numbers, it means it's invalid
    {
        checkcardtype(n, check, len); // Checks which card type it is with conditions
    }
}


// Functions

int cardlength(long n) // Function of the length of a card
{
    long count = 10; // This is used to divide the card number inside a for
    float x = 1; // Initialized the variable x, used as a condition in the for loop
    int len; // Initialized the variable that'll carry the length of the card

    for (int k = 1; x > 0; k++) // length of card
    {
        x = n / count; // This is used to stop the loop when it has counted all the bits of the card
        count = count * 10; // Multiplied by 10 to take into account the next bit
        len = k; // When the loop ends, it'll take the last value, aka the length of the card
    }
    printf("len %i\n", len);
    return len;
}

int checksum(long n, int len) // Checks with the Luhn's Algorithm. Returns the first two digits of the card
{
    int check = 0; // Variable where the first two digits will be stored
    int sum2 = 0; // Total sum
    long left = 0; // What's leftover, stores what'll be added
    long count = 100; // Used to identify elements of the cards
    int left1 = 0; // Used when what's leftover has two digits
    int crd1 = 0; // Stores one of the first digits
    int crd2 = 0; // Stores one of the first digits
    int y = 0; // Used for the condition of one of the for loops

    for (int i = 0; i < len / 2 ; i++) // Sum of *2
    {
        left = ((n % count) / (count / 10)) * 2; // Gives me the value multiplied by two
        crd1 = left / 2;
        if (left >= 10) // Used when the digit *2 gives you two digits
        {
            left1 = left % 10;
            left = left / 10;
        }
        sum2 = sum2 + left + left1; // Adds the check algortihmdigits
        count = count * 100; // Changes every cycle by two zeros so it moves to the digit two places before
        left1 = 0; // Changes to zero so it isn't taken into account in the next cycle
    }

    count = 10; // Changed to 10 because the rest of the digits start with the last digit
    if (len % 2 != 0) // Depending if the length is even or odd, the condition changes
    {
        y = len / 2 + 1;
    }
    else if (len % 2 == 0)
    {
        y = len / 2;
    }

    for (int j = 0; j < y; j++) // Sum of the rest of the digits
    {
        left = (n % count) / (count / 10); // Calculates the rest of the digits
        sum2 += left; // Adds them
        count = count * 100; // Changes every cycle by two zeros so it moves to the digit two places before
        crd2 = left; // Other one of the first two digits
    }

    if ((sum2 % 10) == 0) // If the remainder is 0, it is valid
    {
        if (len % 2 == 0) // If the length is even, the first digits are stored in different variables
        {
            check = (crd1 * 10) + (crd2); // Stores the first two digits
        }
        else // If the length is odd, the first digits are stored in different variables
        {
            check = (crd1) + (crd2 * 10); // Stores the first two digits
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return check;
}

void checkcardtype(long n, int check, int len) // Funciton that determines which type of card it is
{
    if (len == 15 && (check == 34 || check == 37)) // amex
    {
        printf("AMEX\n");
    }
    else if (len == 16 && (check == 51 || check == 52 || check == 53 || check == 54 || check == 55)) // mastercard
    {
        printf("MASTERCARD\n");
    }
    else if ((len == 13 || len == 16) && (check / 10 == 4)) // visa
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}