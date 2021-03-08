#include "app.h"

bool app_run() {
    window win = window_init(WIDTH, HEIGHT, "rpg");

    device_init(&win);

    /* VkPipelineLayout pipeline_layout;
    const pipeline_config_info pipeline_info = pipeline_default_pipeline_config_info(WIDTH, HEIGHT);
    r_pipeline pipeline = pipeline_init(&device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", &pipeline_info); */

    while (!window_should_close(win)) {
        glfwPollEvents();
    }

    device_free();
    /* pipeline_free(pipeline); */
    
    return true;
}