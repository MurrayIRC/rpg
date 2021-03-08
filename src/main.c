#include "app.h"

#include <stdio.h>

int main(void) {
    if (!app_run()) {
        return 1;
    }
    
    return 0;
}