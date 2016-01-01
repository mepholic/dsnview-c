//
// Created by mepholic on 12/21/15.
//

#include "util.h"
#include "dsnapi.h"

int main(int argc, char *argv[]) {
    // Construct relevant URL's
    const char *cfg_url = getConfigURL();
    char *data_url = getDataURL();

    // Initialize strings for data_xml and config_xml
    struct string config_xml;
    if (init_string(&config_xml) == 1) {
        fprintf(stderr, "Failed to allocate config_xml string\n");
        return 1;
    }
    struct string data_xml;
    if (init_string(&data_xml) == 1) {
        fprintf(stderr, "Failed to allocate data_xml string\n");
        return 1;
    }

    // Get config_xml and data_xml contents
    fetchContent(&config_xml, cfg_url);
    fetchContent(&data_xml, data_url);

    xmlSAXHandlerPtr configHandler = initSAXHandler(HAND_CONFIG);
    if (configHandler == NULL) {
        fprintf(stderr, "Failed to initialize config parser handler.");
    }

    parseDSNConfig(NULL, configHandler, &config_xml);

    // Print info
    //printf("Config URL: %s\nData URL: %s\n", cfg_url, data_url);
    //printf("%s\n", config_xml.ptr);

    // Cleanup
    cleanupSAXHandler(configHandler);
    free(config_xml.ptr);
    free(data_xml.ptr);

    return 0;
}
