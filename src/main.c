//
// Created by mepholic on 12/21/15.
//

#include "util.h"
#include "dsnapi.h"

int main(int argc, char *argv[]) {
    // Construct relevant URL's
    const char *cfg_url = get_config_url();
    char *data_url = get_data_url();

    // Initialize strings for data and config
    struct string config;
    if (init_string(&config) == 1) {
        printf("Failed to allocate config string\n");
        return 1;
    }
    struct string data;
    if (init_string(&data) == 1) {
        printf("Failed to allocate data string\n");
        return 1;
    }

    // Get config and data contents
    fetch_content(&config, cfg_url);
    fetch_content(&data, data_url);

    // Test XML stuff
    xmlDocPtr doc;
    xmlNode *root_element = NULL;
    xmlNode *cur_node = NULL;
    doc = xmlReadMemory(data.ptr, (int) data.len, "data.xml", NULL, 0);
    if (doc == NULL) {
        printf("Failed to parse document\n");
        return 1;
    }
    root_element = xmlDocGetRootElement(doc);
    for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s, content: %s\n", cur_node->name, cur_node->content);
        }
    }
    xmlFreeDoc(doc);

    // Print info
    printf("Config URL: %s\nData URL: %s\n", cfg_url, data_url);
    printf("%s\n", data.ptr);

    // Cleanup
    xmlCleanupParser();
    free(config.ptr);

    return 0;
}
