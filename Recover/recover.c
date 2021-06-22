#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    if (argv[1])
    {
        // open file
        FILE *f = fopen(argv[1], "r");
        if (!f)
        {
            printf("file not found.\n");
        }
        
        // starting Variables
        unsigned char buffer[BUFFER_SIZE];
        int jpegCounter = 0;
        FILE *img = NULL;
        char filename[8];
        int fileOpen = 0;
        int length = fread(buffer, BUFFER_SIZE, 1, f);
        
        do
        {
            //check for start of jpeg
            if (buffer[0] == 0xff &&
                buffer[1] == 0xd8 &&
                buffer[2] == 0xff &&
                (buffer[3] & 0xf0) == 0xe0)
            {
                // is there a file open? if so close it and make a new one
                if (fileOpen)
                {
                    fclose(img);
                    jpegCounter++;
                    sprintf(filename, "%03i.jpg", jpegCounter);
                    img = fopen(filename, "w");
                    fwrite(buffer, BUFFER_SIZE, 1, img);
                }
                else
                {
                    sprintf(filename, "%03i.jpg", jpegCounter);
                    img = fopen(filename, "w");
                    fwrite(buffer, BUFFER_SIZE, 1, img);
                    fileOpen = true;
                }
            }
            // if not start of jpeg and a file is open then continue writing
            else
            {
                if (fileOpen)
                {
                    fwrite(buffer, BUFFER_SIZE, 1, img);
                }
            }
            
            length = fread(buffer, BUFFER_SIZE, 1, f);
        }
        while (length > 0);
    }
    else
    {
        printf("no file specified!");
        return 1;
    }
    
}
