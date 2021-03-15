#include "kaneda/app.h"

#include <stdio.h>

void win_leave_console_open(void) {
#ifdef PLATFORM_WIN
    printf("Press enter to continue...\n");
    getchar();
#endif
}

int main(void) {
    if (!app_run()) {
        app_shutdown();
        win_leave_console_open();
        return 1;
    }

    app_shutdown();
    win_leave_console_open();
    return 0;
}