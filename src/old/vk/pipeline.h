#ifndef PIPELINE_H
#define PIPELINE_H

#include "core.h"
#include "device.h"

typedef struct {
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
    uint32 subpass;
} PipelineConfigInfo;

typedef struct {
    RenderDevice *device;
    VkPipeline graphics_pipeline;
    VkShaderModule vert_shader_module;
    VkShaderModule frag_shader_module;
} RenderPipeline;

RenderPipeline pipeline_create(RenderDevice *dev, const char *vert_fp, const char *frag_fp,
                               const PipelineConfigInfo *config_info);
PipelineConfigInfo pipeline_default_config_info(uint32 width, uint32 height);
void pipeline_create_shader_module(RenderDevice *dev, const char *code,
                                   VkShaderModule *shader_module);
void pipeline_destroy(RenderPipeline pipeline);

#endif