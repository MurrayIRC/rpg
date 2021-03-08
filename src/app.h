/* date = February 28th 2021 6:26 pm */

#ifndef APP_H
#define APP_H

#include "window.h"
#include "device.h"
#include "pipeline.h"

#include <stdbool.h>

static const int WIDTH = 800;
static const int HEIGHT = 600;

bool app_run();

/* Pipeline pipeline { device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", Pipeline::DefaultPipelineConfigInfo(WIDTH, HEIGHT) }; */

#endif
