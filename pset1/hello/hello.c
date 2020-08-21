#include <stdio.h>
#include <cs50.h>

void hello(void); // This is used so the main funciton can be on top

int main(void)
{
    // Called the function hello
    hello();
}

// Created a function that returns hello, "name"
void hello(void) // the first void means there is no output, the (void) says there is no input
{
    string name = get_string("What is your name?\n"); // Created a string variable, which prompts the user to enter their name
    printf("hello, %s\n", name); // With the printf function, to print a variable, you use %s for strings and \n to go to the next line
}