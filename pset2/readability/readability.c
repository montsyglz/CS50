#include <cs50.h>
#include <stdio.h>
#include <math.h>

float count_letters(string text);
float count_words(string text);
float count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    // call function that counts the letters
    float letters = count_letters(text);
    // call function that counts the words
    float words = count_words(text);
    // call function that counts the sentences
    float sentences = count_sentences(text);

    // Calculate the grade with the Coleman-Liau index formula
    float L = (letters * 100) / words;
    float S = (sentences * 100) / words;
    float index = (0.0588 * L) - (0.296 * S) - 15.8; // the formula
    if (index < 1) // if the grade is under 1
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n"); // if the grade is over 16
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}

float count_letters(string text) // Function that counts the letters, with conditions check with ascii values
{
    float let = 0; // number of letters in the text

    for (int i = 0; text[i] != '\0'; i++) // this loop will stop when the nol value appears
    {
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122)) // values of ascii
        {
            let++; // counter of the # letters
        }
    }
    return let;
}

float count_words(string text) // Function that counts the words, uses a bool and ascii values
{
    float wor = 1; // counter for number of words, starts in 1 so the first word is included in the counter

    for (int i = 0; text[i] != '\0'; i++) // loop ends when the character is equal to nol
    {
        // in the condition that the character is a letter, and the character before was a space, it'll add a word
        if (((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122)) && (text[i - 1] == 32 || text[i - 1] == 34))
        {
            wor++;
        }
    }
    return wor;
}

float count_sentences(string text) // Function that counts the sentences
{
    float sen = 0; // counter for the number of sentences

    for (int i = 0; text[i] != '\0'; i++) // loop that checks every char to see if it's a period, ! or ?
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63) // values of . ! ? in ascii
        {
            sen++;
        }
    }
    return sen;
}