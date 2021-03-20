#ifndef GRAPHICS_H
#define GRAPHICS_H

Graphics *graphics_create() {
    Graphics *gfx = kamalloc_init(Graphics);

    return gfx;
}

void graphics_destroy(Graphics *graphics) {
    if (graphics == NULL) {
        return;
    }

    // free pipeline data

    // free render pass data

    // free stretchy buffers of their shit

    free(graphics);
    graphics = NULL;
}

#endif