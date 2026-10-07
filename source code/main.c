
/*
    Name              : Shaik Azmeer
    Registered Number : 26015_079
    Project           : MP3 Tag Reader/Editor 
*/

#include <stdio.h>
#include <string.h>
#include "mp3.h"

int main(int argc, char *argv[])
{
    /*
     * Check whether the user has entered
     * at least one command-line argument.
     */
    if (argc < 2)
    {
        printf("ERROR: Invalid arguments\n");
        printf("Use ./a.out --help\n");

        return 1;
    }

    /* -------------------- VIEW OPERATION -------------------- */

    /*
     * Check whether the user selected
     * the view operation using -v.
     */
    if (strcmp(argv[1], "-v") == 0)
    {
        /*
         * View operation requires exactly
         * 3 arguments.
         *
         * argv[0] -> ./a.out
         * argv[1] -> -v
         * argv[2] -> mp3 filename
         */
        if (argc != 3)
        {
            printf("ERROR: Invalid arguments\n");
            printf("Usage: ./a.out -v sample.mp3\n");

            return 1;
        }

        /*
         * Call view_tags() function from view.c
         * and pass the MP3 filename.
         */
        view_tags(argv[2]);
    }

    /* ------------------ EDIT OPERATION ------------------- */

    /*
     * Check whether the user selected
     * the edit operation using -e.
     */
    else if (strcmp(argv[1], "-e") == 0)
    {
        /*
         * Edit operation requires exactly
         * 5 arguments.
         *
         * argv[0] -> ./a.out
         * argv[1] -> -e
         * argv[2] -> edit option
         * argv[3] -> new data
         * argv[4] -> mp3 filename
         */
        if (argc != 5)
        {
            printf("ERROR: Invalid arguments\n");
            printf("Usage: ./a.out -e option new_data sample.mp3\n");

            return 1;
        }

        /*
         * Call edit_tags() function from edit.c.
         *
         * argv[2] -> option
         * argv[3] -> new data
         * argv[4] -> MP3 filename
         */
        if (edit_tags(argv[2], argv[3], argv[4]) == 0)
        {
            if (strcmp(argv[2], "-t") == 0)
            {
                printf("Title updated to: %s\n", argv[3]);
            }
            else if (strcmp(argv[2], "-a") == 0)
            {
                printf("Artist updated to: %s\n", argv[3]);
            }
            else if (strcmp(argv[2], "-A") == 0)
            {
                printf("Album updated to: %s\n", argv[3]);
            }
            else if (strcmp(argv[2], "-m") == 0)
            {
                printf("Genre updated to: %s\n", argv[3]);
            }
            else if (strcmp(argv[2], "-y") == 0)
            {
                printf("Year updated to: %s\n", argv[3]);
            }
            else if (strcmp(argv[2], "-c") == 0)
            {
                printf("Comment updated to: %s\n", argv[3]);
            }

            printf("Tag updated successfully\n");
        }
    }

    /* ---------- HELP OPERATION ---------- */

    /*
     * Check whether the user requested
     * help using --help.
     */
    else if (strcmp(argv[1], "--help") == 0)
    {
        printf("\nMP3 TAG READER AND EDITOR\n");
        printf("------------------------------------------------------------------------------------\n");

        /*
         * --help should not have any
         * additional arguments.
         */
        if (argc != 2)
        {
            printf("ERROR: Invalid arguments for help\n");
            printf("Usage: ./a.out --help\n");

            return 1;
        }

        /*
         * Display the available commands
         * and their usage.
         */
        printf("USAGE :\n");

        printf("To view please pass like: ./a.out -v mp3filename\n");

        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");

        printf("------------------------------------------------------------------------------------\n");
    }

    /* ---------- INVALID OPTION ---------- */

    /*
     * If the first argument is not
     * -v, -e, or --help, show an error.
     */
    else
    {
        printf("ERROR: Invalid option\n");
        printf("Use ./a.out --help\n");

        return 1;
    }

    return 0;
}


/*

Description : 
## 1.Introduction :

The MP3 Tag Reader and Editor is a C-based application used to read and modify 
metadata of MP3 files that follow the ID3v2.3 format. It allows users to view 
and edit details like title, artist, album, year, genre, and comments.

## 2. Objective
To implement file handling in C
To understand MP3 metadata (ID3 tags)
To perform reading and editing of binary file data

## 3. Features

View Mode (-v)

Displays:

Title
Artist
Album
Year
Content (Genre)
Comment
Edit Mode (-e)

Modify specific fields:

-t → Title
-a → Artist
-A → Album
-y → Year
-m → Content
-c → Comment

## 4. Working Principle
The program reads the ID3 header and validates the MP3 file
Extracts metadata from frames like TIT2, TPE1, etc.
In edit mode:
Creates a temporary file
Updates selected tag
Copies remaining data
Replaces original file

## 5. Structure Used

struct MP3
{
    char *mp3_filename;
    FILE *org_mp3_fptr;
    FILE *dup_mp3_fptr;
};

## 6. Key Concepts
Binary file handling (fread, fwrite)
Command-line arguments
Dynamic memory allocation
Endianness conversion
String processing

## 7. Usage
View:
./a.out -v sample.mp3
Edit:
./a.out -e -t "New Title" sample.mp3

## 8. Advantages
Simple and efficient
No external libraries required
Direct metadata manipulation

## 9. Limitations
Supports only ID3v2.3
Limited number of tags
Command-line based

## 10. Conclusion
This project demonstrates how to read and edit MP3 metadata using C, helping 
in understanding file structures, binary data handling, and real-world 
application development.

*/