#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // the rows
    {
        for (int j = 0; j < width; j++) // the columns
        {
            float avg;
            float n = 3;
            avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / n; // get the average of all the red, green and blue
            image[i][j].rgbtBlue = (int) round(avg); // change all the colors to the same avg
            image[i][j].rgbtGreen = (int) round(avg);
            image[i][j].rgbtRed = (int) round(avg);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE row_image[height][width]; // temporary array to store the new order of a row
    for (int i = 0; i < height; i++) // the rows
    {
        for (int j = 0; j < width; j++) // the columns
        {
            row_image[i][j] = image[i][width - (j + 1)]; // sets the new array
        }
    }

    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l] = row_image[k][l];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float redavg = 0;
    float blueavg = 0;
    float greenavg = 0;
    float n = 0;
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++) // the rows
    {
        for (int j = 0; j < width; j++) // the columns
        {
            // i - 1, j - 1
            if (i != 0 && j != 0)
            {
                n++;
                redavg += image[i - 1][j - 1].rgbtRed;
                blueavg += image[i - 1][j - 1].rgbtBlue;
                greenavg += image[i - 1][j - 1].rgbtGreen;
            }

            // i - 1, j
            if (i != 0)
            {
                n++;
                redavg += image[i - 1][j].rgbtRed;
                blueavg += image[i - 1][j].rgbtBlue;
                greenavg += image[i - 1][j].rgbtGreen;
            }

            // i - 1, j + 1
            if (i != 0 && j != width - 1)
            {
                n++;
                redavg += image[i - 1][j + 1].rgbtRed;
                blueavg += image[i - 1][j + 1].rgbtBlue;
                greenavg += image[i - 1][j + 1].rgbtGreen;
            }

            // i, j - 1
            if (j != 0)
            {
                n++;
                redavg += image[i][j - 1].rgbtRed;
                blueavg += image[i][j - 1].rgbtBlue;
                greenavg += image[i][j - 1].rgbtGreen;
            }

            // i, j
            n++;
            redavg += image[i][j].rgbtRed;
            blueavg += image[i][j].rgbtBlue;
            greenavg += image[i][j].rgbtGreen;

            // i, j + 1
            if (j != width - 1)
            {
                n++;
                redavg += image[i][j + 1].rgbtRed;
                blueavg += image[i][j + 1].rgbtBlue;
                greenavg += image[i][j + 1].rgbtGreen;
            }

            // i + 1, j - 1
            if (i != height - 1 && j != 0)
            {
                n++;
                redavg += image[i + 1][j - 1].rgbtRed;
                blueavg += image[i + 1][j - 1].rgbtBlue;
                greenavg += image[i + 1][j - 1].rgbtGreen;
            }

            // i + 1, j
            if (i != height - 1)
            {
                n++;
                redavg += image[i + 1][j].rgbtRed;
                blueavg += image[i + 1][j].rgbtBlue;
                greenavg += image[i + 1][j].rgbtGreen;
            }

            // i + 1, j + 1
            if (i != height - 1 && j != width - 1)
            {
                n++;
                redavg += image[i + 1][j + 1].rgbtRed;
                blueavg += image[i + 1][j + 1].rgbtBlue;
                greenavg += image[i + 1][j + 1].rgbtGreen;
            }

            // Final Step - change the colors
            copy[i][j].rgbtRed = (int) round(redavg / n);
            copy[i][j].rgbtBlue = (int) round(blueavg / n);
            copy[i][j].rgbtGreen = (int) round(greenavg / n);
            n = 0;
            redavg = 0;
            blueavg = 0;
            greenavg = 0;
        }
    }

    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l] = copy[k][l];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    typedef struct
    {
        float red;
        float blue;
        float green;
    }
    color;

    color Gx;
    color Gy;
    float ValueRed, ValueBlue, ValueGreen;
    RGBTRIPLE copy[height][width];

    Gx.red = 0;
    Gx.blue = 0;
    Gx.green = 0;
    Gy.red = 0;
    Gy.blue = 0;
    Gy.green = 0;


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // i - 1, j - 1
            if (i != 0 && j != 0)
            {
                // Gx horizontal
                Gx.red += (-1 * image[i - 1][j - 1].rgbtRed);
                Gx.blue += (-1 * image[i - 1][j - 1].rgbtBlue);
                Gx.green += (-1 * image[i - 1][j - 1].rgbtGreen);
                // Gy vertical
                Gy.red += -1 * image[i - 1][j - 1].rgbtRed;
                Gy.blue += -1 * image[i - 1][j - 1].rgbtBlue;
                Gy.green += -1 * image[i - 1][j - 1].rgbtGreen;
            }

            // i - 1, j
            if (i != 0)
            {
                // Gx horizontal
                Gx.red += 0;
                Gx.blue += 0;
                Gx.green += 0;
                // Gy vertical
                Gy.red += -2 * image[i - 1][j].rgbtRed;
                Gy.blue += -2 * image[i - 1][j].rgbtBlue;
                Gy.green += -2 * image[i - 1][j].rgbtGreen;
            }

            // i - 1, j + 1
            if (i != 0 && j != width - 1)
            {
                // Gx horizontal
                Gx.red += 1 * image[i - 1][j + 1].rgbtRed;
                Gx.blue += 1 * image[i - 1][j + 1].rgbtBlue;
                Gx.green += 1 * image[i - 1][j + 1].rgbtGreen;
                // Gy vertical
                Gy.red += -1 * image[i - 1][j + 1].rgbtRed;
                Gy.blue += -1 * image[i - 1][j + 1].rgbtBlue;
                Gy.green += -1 * image[i - 1][j + 1].rgbtGreen;
            }

            // i, j - 1
            if (j != 0)
            {
                // Gx horizontal
                Gx.red += -2 * image[i][j - 1].rgbtRed;
                Gx.blue += -2 * image[i][j - 1].rgbtBlue;
                Gx.green += -2 * image[i][j - 1].rgbtGreen;
                // Gy vertical
                Gy.red += 0;
                Gy.blue += 0;
                Gy.green += 0;
            }

            // i, j
            // there's nothing to be added

            // i, j + 1
            if (j != width - 1)
            {
                // Gx horizontal
                Gx.red += (2 * image[i][j + 1].rgbtRed);
                Gx.blue += (2 * image[i][j + 1].rgbtBlue);
                Gx.green += (2 * image[i][j + 1].rgbtGreen);
                // Gy vertical
                Gy.red += 0;
                Gy.blue += 0;
                Gy.green += 0;
            }

            // i + 1, j - 1
            if (i != height - 1 && j != 0)
            {
                // Gx horizontal
                Gx.red += -1 * image[i + 1][j - 1].rgbtRed;
                Gx.blue += -1 * image[i + 1][j - 1].rgbtBlue;
                Gx.green += -1 * image[i + 1][j - 1].rgbtGreen;
                // Gy vertical
                Gy.red += 1 * image[i + 1][j - 1].rgbtRed;
                Gy.blue += 1 * image[i + 1][j - 1].rgbtBlue;
                Gy.green += 1 * image[i + 1][j - 1].rgbtGreen;
            }

            // i + 1, j
            if (i != height - 1)
            {
                // Gx horizontal
                Gx.red += 0;
                Gx.blue += 0;
                Gx.green += 0;
                // Gy vertical
                Gy.red += 2 * image[i + 1][j].rgbtRed;
                Gy.blue += 2 * image[i + 1][j].rgbtBlue;
                Gy.green += 2 * image[i + 1][j].rgbtGreen;
            }

            // i + 1, j + 1
            if (i != height - 1 && j != width - 1)
            {
                // Gx horizontal
                Gx.red += 1 * image[i + 1][j + 1].rgbtRed;
                Gx.blue += 1 * image[i + 1][j + 1].rgbtBlue;
                Gx.green += 1 * image[i + 1][j + 1].rgbtGreen;
                // Gy vertical
                Gy.red += 1 * image[i + 1][j + 1].rgbtRed;
                Gy.blue += 1 * image[i + 1][j + 1].rgbtBlue;
                Gy.green += 1 * image[i + 1][j + 1].rgbtGreen;
            }
            // Do gx^2 + gy^2 for each color, add <=255, change the red, bl and gr for that pixel
            ValueRed = sqrt((Gx.red * Gx.red) + (Gy.red * Gy.red));
            if (ValueRed > 255)
            {
                ValueRed = 255;
            }
            ValueBlue = sqrt((Gx.blue * Gx.blue) + (Gy.blue * Gy.blue));
            if (ValueBlue > 255)
            {
                ValueBlue = 255;
            }
            ValueGreen = sqrt((Gx.green * Gx.green) + (Gy.green * Gy.green));
            if (ValueGreen > 255)
            {
                ValueGreen = 255;
            }

            copy[i][j].rgbtRed = (int) round(ValueRed);
            copy[i][j].rgbtBlue = (int) round(ValueBlue);
            copy[i][j].rgbtGreen = (int) round(ValueGreen);

            // Restart values to zero
            Gx.red = 0;
            Gx.blue = 0;
            Gx.green = 0;
            Gy.red = 0;
            Gy.blue = 0;
            Gy.green = 0;
        }
    }

    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l] = copy[k][l];
        }
    }

    return;
}