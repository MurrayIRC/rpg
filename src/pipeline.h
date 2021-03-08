#ifndef PIPELINE_H
#define PIPELINE_H

#include "device.h"

typedef struct pipeline_config_info {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewport_info;
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
    VkPipelineRasterizationStateCreateInfo rasterization_info;
    VkPipelineMultisampleStateCreateInfo multisample_info;
    VkPipelineColorBlendAttachmentState color_blend_attachment;
    VkPipelineColorBlendStateCreateInfo color_blend_info;
    VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;
    uint32_t subpass;
} pipeline_config_info;

typedef struct r_pipeline {
    r_device *device;
    VkPipeline graphics_pipeline;
    VkShaderModule vert_shader_module;
    VkShaderModule frag_shader_module;
} r_pipeline;

r_pipeline pipeline_init(r_device *dev, const char *vert_fp, const char *frag_fp, const pipeline_config_info* config_info);
pipeline_config_info pipeline_default_pipeline_config_info(uint32_t width, uint32_t height);
void pipeline_free(r_pipeline pipeline);
void pipeline_create_shader_module(r_device *dev, const char *code, VkShaderModule *shader_module);


#endif