//
// Created by mepholic on 12/21/15.
//

#include "dsnapi.h"

int fetch_page(char *url, size_t (*cb)(char *content, size_t size, size_t nmemb, void *userdata)) {
    CURL *curl = curl_easy_init();

    if (curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            return res;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_cleanup(curl);
        return res;
    } else {
        printf("Curl initialization failed.\n");
        return 1;
    }
}

size_t print_page(char *content, size_t size, size_t nmemb, void *userdata) {
    size_t page_size = size * nmemb;

    if (page_size != 0) {
        printf("%s\n", content);
    }

    return page_size;
}