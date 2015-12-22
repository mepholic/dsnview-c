//
// Created by mepholic on 12/21/15.
//

#include "dsnapi.h"

int main(int argc, char *argv[]) {
    struct string data;
    init_string(&data);

    const char *cfg_url = get_config_url();
    char *data_url = get_data_url();

    fetch_content(&data, data_url);
    free(data_url);

    printf("%s\n", data.ptr);
    free(data.ptr);

    return 0;
}
