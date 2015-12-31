//
// Created by mepholic on 12/21/15.
//

#include "util.h"
#include "dsnapi.h"

// Define NASA URL's
#define CONFIG_URL "https://eyes.nasa.gov/dsn/config.xml"
#define DATA_URL   "https://eyes.nasa.gov/dsn/data/dsn.xml?r="

// Return configuration URL
const char *getConfigURL() {
    return CONFIG_URL;
}

// Construct the URL for data
char *getDataURL() {
    // Get NASA Time: seconds from epoch divided by 5
    struct timeval t;
    gettimeofday(&t, NULL);
    __time_t nasa_time = t.tv_sec / 5;

    // Find length of URL and NASA Time
    size_t url_len = strlen(DATA_URL);
    int time_len = (int) log10(nasa_time) + 1;

    // Format URL and NASA Time, and store in allocated space
    static char data_url[64];
    char *p_url = data_url;
    sprintf(p_url, "%s%lld", DATA_URL, (long long) nasa_time);
    data_url[url_len + time_len] = '\0';

    return p_url;
}

// Callback for storing page contents
size_t curlWriteFunctionCB(void *content, size_t size, size_t nmemb, struct string *data) {
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
int fetchContent(struct string *data, const char *url) {
    CURL *curl = curl_easy_init();

    if (curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunctionCB);
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

// Parse DSN XML content
int parseDSNConfig(void **stations, struct string *configXML) {

    // Do it better with SAX




    // Test XML stuff
    xmlDocPtr config;
    xmlNode *root_element = NULL;
    xmlNode *cur_node = NULL;
    xmlAttr *cur_prop = NULL;

    config = xmlReadMemory(configXML->ptr, (int) configXML->len, "config.xml", NULL, 0);
    if (config == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        return 1;
    }

    root_element = xmlDocGetRootElement(config);
    if (xmlStrEqual(root_element->name, (const xmlChar *) "config")) {
        for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
            if (cur_node->type == XML_ELEMENT_NODE) {
                printf("type: %s\n", cur_node->name);
                if (xmlStrEqual(cur_node->name, (const xmlChar *) "sites")) {
                    for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next) {
                        if (xmlStrEqual(cur_node->name, (const xmlChar *) "site") && cur_node->properties->children) {
                            printf("\t%s\n", cur_node->name);
                            for (cur_prop = cur_node->properties; cur_prop; cur_prop = cur_prop->next) {
                                printf("\t\t%s: %s\n", cur_prop->name, cur_prop->children->content);
                            }
                        }
                    }
                }
            }
        }
    }
    xmlFreeDoc(config);

    xmlCleanupParser();

    return 0;
}

