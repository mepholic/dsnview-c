//
// Created by mepholic on 12/21/15.
//

#include "dsnapi.h"

// Set constants for NASA URL's
const char *g_config_url = "https://eyes.nasa.gov/dsn/config.xml";
const char *g_data_url   = "https://eyes.nasa.gov/dsn/data/dsn.xml?r=";

// Return configuration URL
const char *get_config_url() {
    return g_config_url;
}

// Construct the URL for data
char *get_data_url() {
    // Get NASA Time: seconds from epoch divided by 5
    struct timeval t;
    gettimeofday(&t, NULL);
    __time_t nasa_time = t.tv_sec / 5;

    // Find length of URL and NASA Time
    size_t url_len = strlen(g_data_url);
    int time_len = (int) log10(nasa_time) + 1;

    // Allocate space for URL
    char *data_url = malloc(sizeof(char) * (url_len + time_len));
    if (data_url == NULL) {
        fprintf(stderr, "Failed to malloc() in data URL function.\n");
        // TODO: Handle error condition
    }

    // Format URL and NASA Time, and store in allocated space
    sprintf(data_url, "%s%lld", g_data_url, (long long) nasa_time);

    return data_url;
}

// Callback for storing page contents
size_t curl_writefunction_cb(void *content, size_t size, size_t nmemb, struct string *data) {
    // Calculate content size from cURL, and add content length to string length
    size_t csize = size * nmemb;
    size_t data_end = data->len + csize;

    // Reallocate pointer memory to reflect new size
    data->ptr = realloc(data->ptr, data_end + 1);
    if (data->ptr == NULL) {
        fprintf(stderr, "Failed to realloc() string in cURL write callback.\n");
        // TODO: Handle error condition
    }

    // Figure out next address to write at, and copy the content in
    void *next_addr = data->ptr + data->len;
    memcpy(next_addr, content, csize);

    // Null terminate string, and adjust length
    data->ptr[data_end] = '\0';
    data->len = data_end;

    return csize;
}

// Grab a page's contents with cURL, send to callback
int fetch_content(struct string *data, const char *url) {
    CURL *curl = curl_easy_init();

    if (curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefunction_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            return res;
        }

        curl_easy_cleanup(curl);
        return res;
    } else {
        fprintf(stderr, "Curl initialization failed.\n");
        return 1;
    }
}

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