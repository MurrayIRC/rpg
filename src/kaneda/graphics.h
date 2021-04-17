#ifndef GRAPHICS_H
#define GRAPHICS_H

Graphics *graphics_create() {
    Graphics *gfx = kamalloc_init(Graphics);

    Platform *platform = engine()->platform;
    glViewport(0, 0, platform->window.render_size.width, platform->window.render_size.height);

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