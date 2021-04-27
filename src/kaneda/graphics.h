#ifndef GRAPHICS_H
#define GRAPHICS_H

Graphics *graphics_create() {
    Graphics *gfx = kamalloc_init(Graphics);

    Platform *platform = engine()->platform;
#ifdef PLATFORM_WINDOWS

#else
    glViewport(0, 0, platform->window.render_size.width, platform->window.render_size.height);
#endif

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