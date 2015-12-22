//
// Created by mepholic on 12/21/15.
//

#ifndef DSNVIEW_DSNAPI_H
#define DSNVIEW_DSNAPI_H

#include <string.h>
#include <malloc.h>
#include <sys/time.h>

#include <math.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// My structures
struct string {
    char *ptr;
    size_t len;
};

// My function declarations
const char *get_config_url();
char *get_data_url();
int fetch_content(struct string *data, const char *url);
size_t curl_writefunction_cb(void *content, size_t size, size_t nmemb, struct string *data);
int init_string(struct string *s);
#endif //DSNVIEW_DSNAPI_H
