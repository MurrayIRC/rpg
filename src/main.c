#include "kaneda/app.h"

#include <stdio.h>

int main(void) {
    if (!app_run()) {
        printf("Press enter to continue...\n");
        getchar();

        return 1;
    }

    printf("Press enter to continue...\n");
    getchar();

    return 0;
}