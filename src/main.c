#include "kaneda/app.h"

#include <stdio.h>

int main(void) {
    if (!app_run()) {
#ifdef PLATFORM_WIN
        printf("Press enter to continue...\n");
        getchar();
#endif

        return 1;
    }

#ifdef PLATFORM_WIN
    printf("Press enter to continue...\n");
    getchar();
#endif

    return 0;
}