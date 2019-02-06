// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    float num = fraction[0] - '0';
    float den = fraction[2] - '0';
    return (8 * (num/den));
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int letter; // This whole entire structure determines how many steps away each letter note is from A
    if (note[0] == 'A')
    {
        letter = 0;
    }
    if (note[0] == 'B')
    {
        letter = 2;
    }
    if (note[0] == 'G')
    {
        letter = -2;
    }
    if (note[0] == 'F')
    {
        letter = -4;
    }
    if (note[0] == 'E')
    {
        letter = -5;
    }
    if (note[0] == 'D')
    {
        letter = -7;
    }
    if (note[0] == 'C')
    {
        letter = -9;
    }

    if (strlen(note) == 2)
    {
            int true_octave = note[1] - 48; // true_octave is the octave corrected for ASCII value
            int multiplier = (true_octave - 4) * 12; // Multiplies number of octaves away from 4 by 12.
            float hertz = 440 * (pow(2, (letter + multiplier)/12.0));
            return round(hertz);
    }

    if (strlen(note) == 3) // Accidentals
    {
            int accidental;
            if (note[1] == '#')
            {
                accidental = 1;
            }
            if (note[1] == 'b')
            {
                accidental = -1;
            }
            int true_octave = note[2] - 48; // true_note is the octave corrected for ASCII value in this case note[2]
            int multiplier = (true_octave - 4) * 12; // Multiplies number of octaves away from 4 by 12.
            float power = ((multiplier + accidental + letter)/12.0);
            float hertz = 440 * pow(2, power);
            return round(hertz);
    }
    else // Error handling for notes of neither 2 or 3 length.
    {
        return 1;
    }
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strcmp(s, ""))
    {
        return false;
    }
    else
    {
        return true;
    }
}
