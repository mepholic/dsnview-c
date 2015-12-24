//
// Created by mepholic on 12/24/15.
//

#ifndef DSNVIEW_UTIL_H
#define DSNVIEW_UTIL_H

#include <stdio.h>
#include <malloc.h>

// My structures
struct string {
    char *ptr;
    size_t len;
};

// Function Definitions
int init_string(struct string *s);

#endif //DSNVIEW_UTIL_H
