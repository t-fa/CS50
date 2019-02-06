// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4) // ensures exactly 4 command line arguemtns
    {
        fprintf(stderr, "Usage: n infile outfile\n");
        return 1;
    }

    // converts n to int
    int n = atoi(argv[1]);

    // ensures n cannot exceed 100 and is greater than 0
    if (n < 1 || n > 100)
    {
        fprintf(stderr, "Usage: n must be a positive integer less than or equal too 100.\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r"); // opens file in read mode. Note: FILE * is just a file pointer, in this case pointing to inptr
    if (inptr == NULL) // ensures file exists - fopen returns null if this step is not performed
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr); // if outptr does not exist, or is NULL, close inptr
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3; // confusing because I thought it'd just write a new file?
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf; // "creates" variable bf
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr); // fread(address of bf, size of bf, 1 time, file inptr)

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi; // "creates" variable bi
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr); // fread(address of bi, size of bi, 1 time, file inptr)

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // expands width and height in BITMAPINFOHEADER
    int oldbiwidth = bi.biWidth;
    int oldbiheight = bi.biHeight;
    bi.biWidth *= n;
    bi.biHeight *= n;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int oldpadding = (4 - (oldbiwidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // Saving myself extra math, this equation determines exactly how many pixels of padding are needed

    // calculate new headers
    bi.biSizeImage = (sizeof(RGBTRIPLE) * bi.biWidth + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // declare an array of RGBTRIPLE type
    RGBTRIPLE *row = malloc(sizeof(RGBTRIPLE) * bi.biWidth);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldbiheight); i < biHeight; i++) // i refers to each row
    {

        // iterate over pixels in scanline
        for (int j = 0; j < oldbiwidth; j++) // j refers to each pixel
        {
            // temporary storage
            RGBTRIPLE triple; // "creates" variable triple

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //write array n times
            for (int l = 0; l <n; l++)
            {
                row[l + (j * n)] = triple;
            }
        }

        // skip over padding, if any
        // in English, if a scanline needs 3 pixels of padding, this rewinds the cursor by 3
        fseek(inptr, oldpadding, SEEK_CUR);

        for (int p = 0; p < n; p++) // write row n times
        {
            fwrite(row, sizeof(RGBTRIPLE), bi.biWidth, outptr);
            // write outfile padding
            for (int k = 0; k < padding; k++)
                fputc(0x00, outptr);
        }
    }

    // because malloc
    free(row);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
