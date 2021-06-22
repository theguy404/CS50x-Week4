#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = image[i][j].rgbtRed +
                            image[i][j].rgbtGreen +
                            image[i][j].rgbtBlue;
            
            average = round(average / 3);
            
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        float widthToCheck = floor(width / 2);
        
        for (int j = 0; j < widthToCheck; j++)
        {
            int farPixel = width - j;
            farPixel = farPixel - 1;
            
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][farPixel];
            image[i][farPixel] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmpimage[height][width];
    
    // loop through the height of the image
    for (int i = 0; i < height; i++)
    {
        // loop through the width of the image
        for (int j = 0; j < width; j++)
        {
            int pixelCount = 0;
            float red = 0;
            float green = 0;
            float blue = 0;
            
            // loop through the height of 3 pixels around chosen pixel
            for (int h = -1; h < 2; h++)
            {
                // loop through the width of the 3 pixels around chosen pixel
                for (int w = -1; w < 2; w++)
                {
                    if (i + h > -1 && j + w > -1 && i + h < height && j + w < width) 
                    {
                        pixelCount++;
                        red += image[i + h][j + w].rgbtRed;
                        green += image[i + h][j + w].rgbtGreen;
                        blue += image[i + h][j + w].rgbtBlue;
                    }
                }
            }
            
            tmpimage[i][j].rgbtRed = round(red / pixelCount);
            tmpimage[i][j].rgbtGreen = round(green / pixelCount);
            tmpimage[i][j].rgbtBlue = round(blue / pixelCount);
            
        }
    }
    
    // overwrite image array with tmpimage array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmpimage[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gxArray[3][3];
    int gyArray[3][3];
    RGBTRIPLE tmpimage[height][width];
    
    //build gxArray
    gxArray[0][0] = -1;
    gxArray[0][1] = 0;
    gxArray[0][2] = 1;
    gxArray[1][0] = -2;
    gxArray[1][1] = 0;
    gxArray[1][2] = 2;
    gxArray[2][0] = -1;
    gxArray[2][1] = 0;
    gxArray[2][2] = 1;
    
    //build gyArray
    gyArray[0][0] = -1;
    gyArray[0][1] = -2;
    gyArray[0][2] = -1;
    gyArray[1][0] = 0;
    gyArray[1][1] = 0;
    gyArray[1][2] = 0;
    gyArray[2][0] = 1;
    gyArray[2][1] = 2;
    gyArray[2][2] = 1;
    
    // loop through pixels of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gxValueRed = 0;
            float gxValueBlue = 0;
            float gxValueGreen = 0;
            
            float gyValueRed = 0;
            float gyValueBlue = 0;
            float gyValueGreen = 0;
            
            //loops through the 9 surrounding pixels
            for (int h = 0; h < 3; h++)
            {
                for (int w = 0; w < 3; w++)
                {
                    if (i + h - 1 > - 1 && j + w - 1 > - 1 && i + h - 1 < height && j + w - 1 < width) 
                    {
                        gxValueRed += gxArray[h][w] * image[i + h - 1][j + w - 1].rgbtRed;
                        gxValueBlue += gxArray[h][w] * image[i + h - 1][j + w - 1].rgbtBlue;
                        gxValueGreen += gxArray[h][w] * image[i + h - 1][j + w - 1].rgbtGreen;
                        
                        gyValueRed += gyArray[h][w] * image[i + h - 1][j + w - 1].rgbtRed;
                        gyValueBlue += gyArray[h][w] * image[i + h - 1][j + w - 1].rgbtBlue;
                        gyValueGreen += gyArray[h][w] * image[i + h - 1][j + w - 1].rgbtGreen;
                    }
                }
            }
            
            // squaring all the values
            gxValueRed = gxValueRed * gxValueRed;
            gxValueBlue = gxValueBlue * gxValueBlue;
            gxValueGreen = gxValueGreen * gxValueGreen;
            
            gyValueRed = gyValueRed * gyValueRed;
            gyValueBlue = gyValueBlue * gyValueBlue;
            gyValueGreen = gyValueGreen * gyValueGreen;
            
            // square root of gx squared and gy squared
            if (sqrt(gxValueRed + gyValueRed) > 255)
            {
                tmpimage[i][j].rgbtRed = 255;
            }
            else
            {
                tmpimage[i][j].rgbtRed = round(sqrt(gxValueRed + gyValueRed));
            }
            
            if (sqrt(gxValueBlue + gyValueBlue) > 255)
            {
                tmpimage[i][j].rgbtBlue = 255;
            }
            else
            {
                tmpimage[i][j].rgbtBlue = round(sqrt(gxValueBlue + gyValueBlue));
            }
            
            if (sqrt(gxValueGreen + gyValueGreen) > 255)
            {
                tmpimage[i][j].rgbtGreen = 255;
            }
            else
            {
                tmpimage[i][j].rgbtGreen = round(sqrt(gxValueGreen + gyValueGreen));
            }
        }
    }
    
    // overwrite image array with tmpimage array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmpimage[i][j];
        }
    }
    return;
}
