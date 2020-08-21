#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int N = 512;

int main(int argc, char *argv[])
{
    // open file to read
    char *infile = argv[1]; // to use the "card.raw" without typing argv[1]
    FILE *Rcard = fopen(infile, "r"); // open the file

    if (argc != 2) // check if user added a name of a file
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    else if (Rcard == NULL) // check if the file was able to open
    {
        fprintf(stderr, "Could not open file %s\n", infile);
        return 1;
    }

    typedef uint8_t BYTE; // type of a byte (8bits)
    BYTE arrRead[N]; // the buffer where what's read is stored
    FILE *Wcard = NULL; // initialize the file where the jpeg'll be stored
    char nameFile[8]; // stores the name of the new jpeg files
    int counter = 0;
    int num; // to check if the fread read less than 512

    // make a loop til the end of the memory card (how to know when it's the end) use feof
    while (1)
    {
        // read 512 bytes into a buffer
        num = fread(arrRead, sizeof(BYTE), N, Rcard);

        // check if it's the end of the file
        if (feof(Rcard) != 0 || num < N)
        {
            break;
        }

        // if it starts with the headers (it's a jpeg)
        if (arrRead[0] == 0xff && arrRead[1] == 0xd8 && arrRead[2] == 0xff && (arrRead[3] & 0xf0) == 0xe0)
        {
            // if it's the first jpeg ever
            if (counter == 0)
            {
                // initiate a file with the sprintf
                sprintf(nameFile, "%03i.jpg", counter);
                // here I should probably open a file to write
                Wcard = fopen(nameFile, "w");
                // start fwrite since it's the first image
                fwrite(arrRead, sizeof(BYTE), N, Wcard);
            }
            // else the rest
            else
            {
                // first I'll close the file
                fclose(Wcard);
                // here I'll use the fwrite
                sprintf(nameFile, "%03i.jpg", counter);
                Wcard = fopen(nameFile, "w");
                fwrite(arrRead, sizeof(BYTE), N, Wcard);
            }
            counter++; // add so the next one has a diff file name
        }
        // else if one jpeg has already been opened and it doesn't have the header
        else if (counter > 0 && Wcard != NULL)
        {
            // if already found a jpeg, keep writing with fwrite
            fwrite(arrRead, sizeof(BYTE), N, Wcard);
        }
        else
        {
            continue;
        }
    }

    // close files
    fclose(Rcard);
    fclose(Wcard);

    return 0;
}
