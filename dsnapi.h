//
// Created by mepholic on 12/21/15.
//

#ifndef DSNVIEW_DSNAPI_H
#define DSNVIEW_DSNAPI_H

#include <curl/curl.h>

int fetch_page(char *url, size_t (*cb)(char *content, size_t size, size_t nmemb, void *userdata));
size_t print_page(char *content, size_t size, size_t nmemb, void *userdata);
#endif //DSNVIEW_DSNAPI_H
