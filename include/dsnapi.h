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

// Typedefs
typedef enum handType {
    HAND_CONFIG,
    HAND_DATA
} SAXHandleType;

typedef enum retCode {
    RET_OK,
    RET_FAIL
} RetVal;

typedef enum xmlState {
    STATE_DOC_START,
    STATE_ELEM_START,
    STATE_ATTR_DECL,
    STATE_ELEM_END,
    STATE_DOC_END
} StatesEnum;

// Structures
struct ParserState {
    RetVal retVal;
    StatesEnum state;
};

struct dsnSpacecraft {
    char *name;
    char *explorerName;
    char *friendlyName;
    char *thumbnail;
};

struct dsnSignal {
    int  signalDir;
    char *signalType;
    char *signalTypeDebug;
    char *power;
    char *frequency;
    char *dataRate;
    char *spacecraft;
    char *spacecraftId;
};

struct dsnTarget {
    char *name;
    char *id;
    char *uplegRange;
    char *downlegRange;
    struct dsnSignal *upstreams;
    struct dsnSignal *downstreams;
};

struct dsnDish {
    char *name;
    char *friendlyName;
    char *type;
    char *azimuthAngle;
    char *elevationAngle;
    char *windSpeed;
    char *isMSPA;
    char *isArray;
    char *isDDOR;
    char *created;
    char *updated;
    struct dsnTarget *targets;
};

struct dsnStation {
    char *name;
    char *friendlyName;
    char *longitude;
    char *latitude;
    char *timeUTC;
    char *timeZoneOffset;
    struct dsnDish *dishes;
};

// Function declarations
const char *getConfigURL();
char *getDataURL();
int fetchContent(struct string *data, const char *url);
size_t curlWriteFunctionCB(void *content, size_t size, size_t nmemb, struct string *data);

xmlSAXHandlerPtr initSAXHandler(SAXHandleType type);
void cleanupSAXHandler(xmlSAXHandlerPtr ourSAXHandlerPtr);

void configDocStart(void *user_data);
void configDocEnd(void *user_data);
void configElemStart(void *user_data, const xmlChar *name, const xmlChar **attrs);
void configElemEnd(void *user_data, const xmlChar *name);
int parseDSNConfig(void **stations, xmlSAXHandlerPtr configSAXHandler, struct string *configXML);
#endif //DSNVIEW_DSNAPI_H
