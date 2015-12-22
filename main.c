//
// Created by mepholic on 12/21/15.
//

#include "dsnapi.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        fetch_page(argv[1], print_page);
        return 0;
    } else {
        printf("Incorrect number of parameters.\n");
        return 1;
    }
}
