#include "app.h"

bool app_run() {
    window win = window_init(WIDTH, HEIGHT, "rpg");

    vk_device device = vkd_init(&win);

    VkPipelineLayout pipeline_layout;
    const pipeline_config_info vkp_info = vkp_default_pipeline_config_info(WIDTH, HEIGHT);
    vk_pipeline pipeline = vkp_init(&device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", &vkp_info);

    while (!window_should_close(win)) {
        glfwPollEvents();
    }

    vkd_free(device);
    vkp_free(pipeline);
    
    return true;
}