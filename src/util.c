//
// Created by mepholic on 12/24/15.
//

#include "util.h"

// Initialize an empty string
int init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);

    if (s->ptr == NULL) {
        fprintf(stderr, "Failed to malloc()\n");
        return 1;
    }

    s->ptr[0] = '\0';
    return 0;
}