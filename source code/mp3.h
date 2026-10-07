#ifndef MP3_H
#define MP3_H

#include <stdio.h>
#include <string.h>

/* Function declaration for view operation */
int view_tags(char *filename);

/* Function declaration for edit operation */
int edit_tags(char *option, char *new_data, char *filename);

#endif
