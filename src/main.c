#include "app.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    if (!app_run()) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}