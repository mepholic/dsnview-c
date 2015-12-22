//
// Created by mepholic on 12/21/15.
//

#include "dsnapi.h"

int main(int argc, char *argv[]) {
    struct string contents;
    init_string(&contents);

    const char *cfg_url = get_config_url();
    char *data_url = get_data_url();

    fetch_content(&contents, data_url);
    free(data_url);

    printf("%s\n", contents.ptr);
    free(contents.ptr);

    return 0;
}
