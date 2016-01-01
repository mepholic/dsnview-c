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

// SAX Handlers
xmlSAXHandlerPtr initSAXHandler(SAXHandleType type) {
    xmlSAXHandlerPtr ourSAXHandlerPtr = calloc(1, sizeof(xmlSAXHandler));
    if (ourSAXHandlerPtr == NULL) {
        return NULL;
    }

    if (type == HAND_CONFIG) {
        ourSAXHandlerPtr->startDocument = configDocStart;
        ourSAXHandlerPtr->endDocument   = configDocEnd;
        ourSAXHandlerPtr->startElement  = configElemStart;
        ourSAXHandlerPtr->endElement    = configElemEnd;
    } else if (type == HAND_DATA) {
//    ourSAXHandlerPtr->startDocument = &configDocStart;
//    ourSAXHandlerPtr.endDocument   = configDocEnd;
//    ourSAXHandlerPtr.startElement  = configElemStart;
//    ourSAXHandlerPtr.endElement    = configElemEnd;
    }

    return ourSAXHandlerPtr;
}

void cleanupSAXHandler(xmlSAXHandlerPtr ourSAXHandlerPtr) {
    free(ourSAXHandlerPtr);
}

// Config Handlers
void configDocStart(void *user_data) {
    struct ParserState *s = user_data;
    s->state = STATE_DOC_START;
    printf("Starting document\n");
}

void configDocEnd(void *user_data) {
    struct ParserState *s = user_data;
    s->state = STATE_DOC_END;
    printf("Ending document\n");
}

void configElemStart(void *user_data, const xmlChar *name, const xmlChar **attrs) {
    struct ParserState *s = user_data;
    s->state = STATE_ELEM_START;
    printf("Starting element: %s\n", name);
}

void configElemEnd(void *user_data, const xmlChar *name) {
    struct ParserState *s = user_data;
    s->state = STATE_ELEM_END;
    printf("Ending element: %s\n", name);
}

// Parse DSN XML content
int parseDSNConfig(void **stations, xmlSAXHandlerPtr configSAXHandler, struct string *configXML) {

    // Do it better with SAX
    struct ParserState configDocState;

    if (xmlSAXUserParseMemory(configSAXHandler, &configDocState, configXML->ptr, (int) configXML->len) < 0) {
        return RET_FAIL;
    } else {
        return configDocState.retVal;
    }

    return 0;
}

