#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3.h"

/*
 * This function removes extra information from the tag data.
 * Example:
 * "Song Name - [Official Audio]"
 * becomes: "Song Name"
 */
void remove_extra(char *str)
{
    char *p;

    /* Search for " - [" in the string */
    p = strstr(str, " - [");

    /* If found, terminate the string there */
    if (p != NULL)
    {
        *p = '\0';
    }
}


int view_tags(char *filename)
{
    FILE *fp;

    /*
     * ID3 header contains:
     * 3 bytes -> "ID3"
     * 2 bytes -> Version
     * 1 byte  -> Flags
     * 4 bytes -> Tag size
     */
    char id[4];
    char version[2];
    char flag;
    unsigned char size[4];

    /*
     * ID3 frame information:
     *
     * frame      -> Frame ID such as TIT2, TPE1
     * frame_size -> Size of frame data
     */
    char frame[5];
    unsigned char frame_size[4];

    int len;

    // Pointer used to store frame data 
    char *data;


    // Open the MP3 file in binary read mode.
    fp = fopen(filename, "rb");


    /* Check whether the file was opened successfully */
    if (fp == NULL)
    {
        printf("File not found\n");

        return 1;
    }


    /*
     * Read the first 3 bytes from the MP3 file.
     * An ID3v2 file should start with:
     * I D 3
     */
    fread(id, 1, 3, fp);

    /* Add NULL character to make it a string */
    id[3] = '\0';


    // Check whether the file contains an ID3 header.
    if (strcmp(id, "ID3") != 0)
    {
        printf("ID3 tag not found\n");

        fclose(fp);

        return 1;
    }


    /*
     * Read ID3 version.
     *
     * version[0] -> major version
     * version[1] -> minor version
     */
    fread(version, 1, 2, fp);


    // Read ID3 flags.
    fread(&flag, 1, 1, fp);

    // Read the total ID3 tag size.
    fread(size, 1, 4, fp);


    /*
     * Print heading for MP3 tag information.
     */
    printf("\n-------------------------------------------------------------\n");
    printf("                    MP3 TAG DETAILS\n");
    printf("-------------------------------------------------------------\n");


    /*
     * Display ID3 version.
     * Example:
     * ID3v2.3.0
     */
    printf("Version : ID3v2.%d.%d\n", version[0], version[1]);


    /*
     * Read all ID3 frames one by one.
     */
    while (1)
    {
        /*
         * Read 4 bytes for frame ID.
         *
         * Examples:
         * TIT2 -> Title
         * TPE1 -> Artist
         * TALB -> Album
         * TYER -> Year
         * TCON -> Genre
         * COMM -> Comment
         */
        if (fread(frame, 1, 4, fp) != 4)
            break;

        /* Make frame ID a string */
        frame[4] = '\0';


        /*
         * If frame ID is empty,
         * there are no more frames.
         */
        if (frame[0] == '\0')
            break;


        // Read the 4-byte frame size.
        if (fread(frame_size, 1, 4, fp) != 4)
            break;


        // Skip the 2-byte frame flags.
        fseek(fp, 2, SEEK_CUR);


        // Convert the 4 bytes of frame size into one integer value.
        len = (frame_size[0] << 24) | (frame_size[1] << 16) | (frame_size[2] << 8) | frame_size[3];

        // If frame size is zero or negative, stop reading frames.
        if (len <= 0)
            break;


        // Allocate memory to store the frame data.
        // +1 is used for the NULL character.

        data = malloc(len + 1);


        // Check whether memory allocation  was successful.
        if (data == NULL)
        {
            printf("Memory allocation failed\n");

            fclose(fp);

            return 1;
        }

        /*
         * Read the actual frame data
         * from the MP3 file.
         */
        fread(data, 1, len, fp);

        /*
         * Add NULL character so that
         * data can be treated as a string.
         */
        data[len] = '\0';

        // TIT2 = Title
        if (strcmp(frame, "TIT2") == 0)
        {
            remove_extra(data + 1);

            printf("Title   : %s\n", data + 1);
        }

        // TPE1 = Artist
        else if (strcmp(frame, "TPE1") == 0)
        {
            remove_extra(data + 1);

            printf("Artist  : %s\n", data + 1);
        }

        // TALB = Album
        else if (strcmp(frame, "TALB") == 0)
        {
            remove_extra(data + 1);

            printf("Album   : %s\n", data + 1);
        }

       // TYER = Year
        else if (strcmp(frame, "TYER") == 0)
        {
            printf("Year    : %s\n", data + 1);
        }

        // TCON = Content/Genre
        else if (strcmp(frame, "TCON") == 0)
        {
            remove_extra(data + 1);

            printf("Content : %s\n", data + 1);
        }

        // COMM = Comment
        else if (strcmp(frame, "COMM") == 0)
        {
            printf("Comment : %s\n", data + 1);
        }

        // Free the memory allocated
        // for the current frame.
        free(data);
    }

    printf("-------------------------------------------------------------\n");

    fclose(fp);

    return 0;
}