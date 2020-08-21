#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

bool validate_key(int argc, string argv[], int upper_case[], int lower_case[]);
string encipher(string argv[], int upper_case[], int lower_case[], string plain_text);

int main(int argc, string argv[])
{
    // MAKE UPPER AND LOWER CASE ARRAYS
    int value = 65; // Value of the upper case A taken from ascii
    int upper_case[26]; // initialized upper case letters array
    int lower_case[26]; // initialized lower case letters array

    // UPPER CASE ARRAY
    for (int i = 0; i < 26; i++)
    {
        upper_case[i] = value + i; // Adds one so it can correpond to the next letter based on ascii
    }

    // LOWER CASE ARRAY
    value = 97; // Changed the value corresponding to the ascii value of the lower case a
    for (int i = 0; i < 26; i++)
    {
        lower_case[i] = value + i;
    }

    // CALL FUNCTION TO VALIDATE THE KEY INPUT
    bool validate = validate_key(argc, argv, upper_case, lower_case);
    if (validate == false) // If it's false, return a value to exit the code
    {
        return 1;
    }

    // INPUT GET_STRING PLAIN TEXT
    string plain_text = get_string("plaintext: ");

    // CALL FUNCTION TO ENCYPHER THE MESSAGE WITH THE KEY
    string cipher_text = encipher(argv, upper_case, lower_case, plain_text);
    printf("ciphertext: %s\n", cipher_text); // Print the cipher text
}

//FUNCTIONS

// Function that validate key inside argv with conditions
bool validate_key(int argc, string argv[], int upper_case[], int lower_case[])
{
    bool val; // variable which will store true if valid, and false if not
    int length; // variable to store the length of the key
    int check = 0; // variable used to check if any letter is reapeated

    if (argc != 2) // If there are more or less than 2 arguments, the input is not valid
    {
        val = false;
        printf("Usage: ./substitution key\n");
        return val;
    }

    for (int i = 0; argv[1][i] != '\0'; i++) // loop to start analizing the key entered in the command line argument
    {
        if ((argv[1][i] >= 65 && argv[1][i] <= 90) || (argv[1][i] >= 97
                && argv[1][i] <= 122)) // check they are alphabetic and do not repeat
        {
            for (int j = 0; j < 26; j++) // This loop checks for repeated letters (case sensetive)
            {
                if (argv[1][i] == upper_case[j]) // Checks the character with the upper case array
                {
                    check += j; // In order for the letters to not be repeated, the sum must be 325
                    break; // Breaks the loop when it enters either condition
                }
                else if (argv[1][i] == lower_case[j]) // Checks the character with the lower case array
                {
                    check += j;
                    break;
                }
                if (argv[1][i] == argv[1][i - 1])
                {
                    check = 0;
                    val = false;
                    printf("Usage: ./substitution key\n");
                    return val;
                }
            }
        }
        else // If it is not an alphabetical character, the function will return false
        {
            val = false;
            printf("Usage: ./substitution key\n");
            return val;
        }
        length = i; // With the last value of the loop, I'll be able to check if it's the right length
    }
    if (length == 25 && check == 325) // validate the length of the key
    {
        val = true;
        return val;
    }
    else if (check != 325)// This would mean it has alphabetical characters, but is still is not valid (length or repeated)
    {
        val = false;
        printf("Key must not contain repeated characters.\n");
        return val;
    }
    else
    {
        val = false;
        printf("Key must contain 26 characters.\n");
        return val;
    }
}

// FUNCTION TO CREATE THE CIPHERED TEXT
string encipher(string argv[], int upper_case[], int lower_case[], string plain_text)
{
    int pos; // Variable used to determine the position of the letter from the key (used in a for)
    bool check; // Used to identify when to put the letter in lower or upper case
    string cipher_text = plain_text; // initialize it equal to the plain text (input)

    for (int i = 0; plain_text[i] != '\0'; i++) // Checks each character in the input of the user
    {
        if ((plain_text[i] >= 65 && plain_text[i] <= 90) || (plain_text[i] >= 97
                && plain_text[i] <= 122)) // Enters if the character is alphabetical
        {
            for (int j = 0; j < 26; j++) // This for is to determine in what position of the alphabet the character is
            {
                if (upper_case[j] == plain_text[i]) // compares it to the upper case alphabet
                {
                    pos = j; // the position is equal to the j of that cycle
                    check = true; // I determined that true meant upper case
                    break; // when it's equal to a letter, the loop can be stopped
                }
                else if (lower_case[j] == plain_text[i])
                {
                    pos = j;
                    check = false; // I determined that false meant lower case
                    break;
                }
            }

            if (check == true) // Enters if it's upper
            {
                cipher_text[i] = toupper((int) argv[1][pos]); // I used the toupper funtion to turn the key to upper case
            }
            else if (check == false) // Enters if it's lower
            {
                cipher_text[i] = tolower((int) argv[1][pos]); // tolower function to turn the key to lower case
            }
        }
    }
    return cipher_text;
}