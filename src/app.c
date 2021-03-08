#include "app.h"
#include "window.h"
#include "device.h"
#include "pipeline.h"

boolean app_run(void) {
    Window *win = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "rpg");
    RenderDevice *device = device_create(win);

    /* VkPipelineLayout pipeline_layout;
    const pipeline_config_info pipeline_info = pipeline_default_config_info(WIDTH, HEIGHT);
    r_pipeline pipeline = pipeline_create(&device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", &pipeline_info); */

    while (!window_should_close(win)) {
        glfwPollEvents();
    }

    /*device_destroy(); */
    /* pipeline_destroy(pipeline); */

    device_destroy(device);
    window_destroy(win);
    
    return TRUE;
}