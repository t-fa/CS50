// recovers missing JPEG memory card data

#include <stdio.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: 1 command line argument (filename)!\n");
        return 1;
    }

    // open command line file
    FILE *infile = fopen(argv[1], "r");
    if (argv[1] == NULL) // ensures file exists - fopen returns null if this step is not performed
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // declare variables
    unsigned char buffer[512]; // for checking first 4 bytes
    int file_counter = 0; // counts number of files for sprintf
    char filename[8]; // array for sprintf
    FILE *newfile; // where jpegs will live

    // read 512 bytes
    while(fread(&buffer, 1, 512, infile) == 512) // want to read one block 512 times. fread returns number of blocks successfully
    //read, so we want to be able to parse less than 512 for EOF
    {
        // bitwise & operator for checking first 4 bytes of JPEG
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (file_counter > 0) // to close previous file after first run of loop
            {
                fclose(newfile); // close previous file before opening new one
            }

            sprintf(filename, "%03i.jpg", file_counter);
            file_counter++;
            newfile = fopen(filename, "w");
            if (newfile == NULL) // ensures file exists - fopen returns null if this step is not performed
            {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 2;
            }
            fwrite(&buffer, 1, 512, newfile);

        }
        // jpeg larger than 1 block
        if (file_counter > 0)
        {
            fwrite(&buffer, 1, 512, newfile);
        }
    }

    fclose(infile);
    fclose(newfile);

    // success
    return 0;
}
