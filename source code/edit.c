#include <stdio.h>
#include <string.h>
#include "mp3.h"

/*
 * Convert integer size into 4 bytes.
 *
 * ID3 frame size is stored using 4 bytes.
 */
void write_size(unsigned char *size, int n)
{
    size[0] = (n >> 24) & 0xff;
    size[1] = (n >> 16) & 0xff;
    size[2] = (n >> 8) & 0xff;
    size[3] = n & 0xff;
}


/*
 * Convert 4 bytes into an integer.
 *
 * This is used to get the size of the current ID3 frame.
 */
int get_size(unsigned char *size)
{
    return ((size[0] << 24) | (size[1] << 16) | (size[2] << 8) | size[3]);
}

/*
 * Copy 'n' bytes from one file to another.
 *
 * fp1 -> source file
 * fp2 -> destination file
 * n   -> number of bytes to copy
 */
void copy_data(FILE *fp1, FILE *fp2, int n)
{
    char buffer[8192];
    int bytes;

    /*
     * Continue until all required
     * bytes are copied.
     */
    while (n > 0)
    {
        /*
         * Copy maximum 8192 bytes at a time.
         */
        bytes = n > 8192 ? 8192 : n;

        /* Read data from original MP3 */
        fread(buffer, 1, bytes, fp1);

        /* Write data into temporary MP3 */
        fwrite(buffer, 1, bytes, fp2);

        /* Reduce remaining byte count */
        n = n - bytes;
    }
}

int edit_tags(char *option, char *new_data, char *filename)
{
    FILE *fp1;
    FILE *fp2;

    /*
     * id   -> stores current frame ID
     * tag  -> stores the tag we want to edit
     * flag -> stores frame flags
     */
    char id[5];
    char tag[5];
    char flag[2];

    // Stores frame size as 4 bytes.
    unsigned char size[4];

    int old_size;
    int new_size;

    // Used to know whether the requested tag was found or not.
    int found = 0;

    // Check which tag the user wants to edit.

    /* -t means Title */
    if (strcmp(option, "-t") == 0)
    {
        strcpy(tag, "TIT2");
    }

    /* -a means Artist */
    else if (strcmp(option, "-a") == 0)
    {
        strcpy(tag, "TPE1");
    }

    /* -A means Album */
    else if (strcmp(option, "-A") == 0)
    {
        strcpy(tag, "TALB");
    }

    /* -m means Genre */
    else if (strcmp(option, "-m") == 0)
    {
        strcpy(tag, "TCON");
    }

    /* -y means Year */
    else if (strcmp(option, "-y") == 0)
    {
        strcpy(tag, "TYER");
    }

    /* -c means Comment */
    else if (strcmp(option, "-c") == 0)
    {
        strcpy(tag, "COMM");
    }

    /* Invalid edit option */
    else
    {
        printf("Invalid option\n");

        return 1;
    }

    /*
     * Open the original MP3 file
     * in binary read mode.
     */
    fp1 = fopen(filename, "rb");

    /* Check whether file opened successfully */
    if (fp1 == NULL)
    {
        printf("File not found\n");

        return 1;
    }

    /*
     * Create a temporary file.
     *
     * We first write the modified data
     * into this file.
     */
    fp2 = fopen("temp.mp3", "wb");

    /* Check whether temporary file was created */
    if (fp2 == NULL)
    {
        printf("Unable to create temporary file\n");

        fclose(fp1);

        return 1;
    }

    /*Copy the first 10 bytes.
     * These 10 bytes contain the
     * ID3 header.*/
    copy_data(fp1, fp2, 10);


    // Read each ID3 frame one by one.
    while (fread(id, 1, 4, fp1) == 4)
    {
        // Add null character so that frame ID can be treated as a string.
        id[4] = '\0';


        // Read the 4-byte frame size.
        if (fread(size, 1, 4, fp1) != 4)
            break;

        // Read the 2-byte frame flags
        if (fread(flag, 1, 2, fp1) != 2)
            break;

        // Convert the 4-byte size into an integer.
        old_size = get_size(size);


        // If frame ID is empty, stop reading.
        if (id[0] == '\0')
            break;


        // Check whether the current frame is the tag requested by the user.
        if (strcmp(id, tag) == 0)
        {
            /*
             * Calculate the size of new data.
             * +1 is for the encoding byte.
             */
            new_size = strlen(new_data) + 1;

            /* Write frame ID */
            fwrite(id, 1, 4, fp2);

            
            // Write the new frame size.
            write_size(size, new_size);

            fwrite(size, 1, 4, fp2);

            /* Write the frame flags */
            fwrite(flag, 1, 2, fp2);


            fputc(0, fp2);

        
            // Write the new tag value.
            fwrite(new_data, 1, strlen(new_data), fp2);

    
            // Skip the old tag data in the original MP3 file.
            fseek(fp1, old_size, SEEK_CUR);


            // Remember that the requested tag has been found.
            found = 1;
        }

        else
        {
            
            // This is not the tag we want to edit.
            // So copy the complete frame without changing it.

            /* Copy frame ID */
            fwrite(id, 1, 4, fp2);

            /* Copy frame size */
            fwrite(size, 1, 4, fp2);

            /* Copy frame flags */
            fwrite(flag, 1, 2, fp2);

            // Copy the original frame data.
            copy_data(fp1, fp2, old_size);
        }
    }

    /*
    After all ID3 frames are processed,
    copy the remaining MP3 audio data. */
    {
        char buffer[8192];
        int bytes;

        while ((bytes = fread(buffer, 1, sizeof(buffer), fp1)) > 0)
        {
            fwrite(buffer, 1, bytes, fp2);
        }
    }

    // Close both files.
    fclose(fp1);
    fclose(fp2);

    
    // If requested tag was not found,  remove temporary file
    if (found == 0)
    {
        printf("Tag not found\n");

        remove("temp.mp3");

        return 1;
    }


    // Delete the original MP3 file.
    remove(filename);

    // Rename temporary file to the original MP3 filename.
    if (rename("temp.mp3", filename) != 0)
    {
        printf("File update failed\n");

        return 1;
    }

    // Everything completed successfully.
    //printf("Tag updated successfully\n");

    return 0;
}