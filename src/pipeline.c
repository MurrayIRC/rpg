#include "pipeline.h"
#include "file.h"
#include "log.h"
#include <assert.h>

RenderPipeline pipeline_create(RenderDevice* dev, const char *vert_fp, const char *frag_fp, const PipelineConfigInfo* config_info) {
    assert(config_info->pipeline_layout != NULL && "Cannot create graphics pipeline: no pipelineLayout provided in config info.");
    assert(config_info->render_pass != NULL && "Cannot create graphics pipeline: no renderPass provided in config info.");

    RenderPipeline pipeline;
    pipeline.device = dev;

    const char* vert_code;
    file vert_file = file_open(vert_fp, FILE_MODE_READ);
    file_read(vert_file, 0, vert_file->size, &vert_code);
    pipeline_create_shader_module(dev, vert_code, &pipeline.vert_shader_module);
    file_close(vert_file);

    const char* frag_code;
    file frag_file = file_open(frag_fp, FILE_MODE_READ);
    file_read(frag_file, 0, frag_file->size, &frag_code);
    pipeline_create_shader_module(dev, frag_code, &pipeline.frag_shader_module);
    file_close(frag_file);

    VkPipelineShaderStageCreateInfo shader_stages[2];
    shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stages[0].module = pipeline.vert_shader_module;
    shader_stages[0].pName = "main";
    shader_stages[0].flags = 0;
    shader_stages[0].pNext = NULL;
    shader_stages[0].pSpecializationInfo = NULL;
    shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stages[1].module = pipeline.frag_shader_module;
    shader_stages[1].pName = "main";
    shader_stages[1].flags = 0;
    shader_stages[1].pNext = NULL;
    shader_stages[1].pSpecializationInfo = NULL;

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = NULL,  /* Optional */
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = NULL /* Optional */
    };

    VkGraphicsPipelineCreateInfo pipeline_info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shader_stages,
        .pVertexInputState = &vertex_input_info,
        .pInputAssemblyState = &config_info->input_assembly_info,
        .pViewportState = &config_info->viewport_info,
        .pRasterizationState = &config_info->rasterization_info,
        .pMultisampleState = &config_info->multisample_info,
        .pColorBlendState = &config_info->color_blend_info,
        .pDynamicState = NULL,                /* Optional */
        .pDepthStencilState = &config_info->depth_stencil_info,
        .layout = config_info->pipeline_layout,
        .renderPass = config_info->render_pass,
        .subpass = config_info->subpass,
        .basePipelineHandle = VK_NULL_HANDLE, /* Optional */
        .basePipelineIndex = -1               /* Optional */
    };

    VkResult result = vkCreateGraphicsPipelines(dev->vk_device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &pipeline.graphics_pipeline);

    if (result != VK_SUCCESS) {
        log_fatal("Failed to create graphics pipeline!\n");
        exit(1);
    }

    vkDestroyShaderModule(dev->vk_device, pipeline.vert_shader_module, NULL);
    pipeline.vert_shader_module = VK_NULL_HANDLE;
    vkDestroyShaderModule(dev->vk_device, pipeline.frag_shader_module, NULL);
    pipeline.frag_shader_module = VK_NULL_HANDLE;

    return pipeline;
}

PipelineConfigInfo pipeline_default_config_info(uint32 width, uint32 height) {
    PipelineConfigInfo config_info = {
        .input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .input_assembly_info.primitiveRestartEnable = VK_FALSE,

        .viewport.x = 0.0f,
        .viewport.y = 0.0f,
        .viewport.width = (float)width,
        .viewport.height = (float)height,
        .viewport.minDepth = 0.0f,
        .viewport.maxDepth = 1.0f,

        .scissor.offset = {0, 0},
        .scissor.extent = {width, height},

        /* Known issue: this creates a self-referencing structure. Fixed in tutorial 05 */
        .viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewport_info.viewportCount = 1,
        .viewport_info.pViewports = &config_info.viewport,
        .viewport_info.scissorCount = 1,
        .viewport_info.pScissors = &config_info.scissor,

        .rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .rasterization_info.depthClampEnable = VK_FALSE,
        .rasterization_info.rasterizerDiscardEnable = VK_FALSE,
        .rasterization_info.polygonMode = VK_POLYGON_MODE_FILL,
        .rasterization_info.lineWidth = 1.0f,
        .rasterization_info.cullMode = VK_CULL_MODE_NONE,
        .rasterization_info.frontFace = VK_FRONT_FACE_CLOCKWISE,
        .rasterization_info.depthBiasEnable = VK_FALSE,
        .rasterization_info.depthBiasConstantFactor = 0.0f,  /* Optional */
        .rasterization_info.depthBiasClamp = 0.0f,           /* Optional */
        .rasterization_info.depthBiasSlopeFactor = 0.0f,     /* Optional */

        .multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .multisample_info.sampleShadingEnable = VK_FALSE,
        .multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .multisample_info.minSampleShading = 1.0f,           /* Optional */
        .multisample_info.pSampleMask = NULL,                /* Optional */
        .multisample_info.alphaToCoverageEnable = VK_FALSE,  /* Optional */
        .multisample_info.alphaToOneEnable = VK_FALSE,       /* Optional */

        .color_blend_attachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT,
        .color_blend_attachment.blendEnable = VK_FALSE,
        .color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,   /* Optional */
        .color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,  /* Optional */
        .color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD,              /* Optional */
        .color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,   /* Optional */
        .color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,  /* Optional */
        .color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD,              /* Optional */

        .color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .color_blend_info.logicOpEnable = VK_FALSE,
        .color_blend_info.logicOp = VK_LOGIC_OP_COPY,  /* Optional */
        .color_blend_info.attachmentCount = 1,
        .color_blend_info.pAttachments = &config_info.color_blend_attachment,
        .color_blend_info.blendConstants[0] = 0.0f,  /* Optional */
        .color_blend_info.blendConstants[1] = 0.0f,  /* Optional */
        .color_blend_info.blendConstants[2] = 0.0f,  /* Optional */
        .color_blend_info.blendConstants[3] = 0.0f,  /* Optional */

        .depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depth_stencil_info.depthTestEnable = VK_TRUE,
        .depth_stencil_info.depthWriteEnable = VK_TRUE,
        .depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS,
        .depth_stencil_info.depthBoundsTestEnable = VK_FALSE,
        .depth_stencil_info.minDepthBounds = 0.0f,  /* Optional */
        .depth_stencil_info.maxDepthBounds = 1.0f,  /* Optional */
        .depth_stencil_info.stencilTestEnable = VK_FALSE,
        .depth_stencil_info.front = {},  /* Optional */
        .depth_stencil_info.back = {},   /* Optional */
    };

    return config_info;
}

void pipeline_create_shader_module(RenderDevice *dev, const char *code, VkShaderModule *shader_module) {
    VkShaderModuleCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = sizeof(code),
        .pCode = (const uint32*)code,
    };

    VkResult result = vkCreateShaderModule(dev->vk_device, &create_info, NULL, shader_module);
    if (result != VK_SUCCESS) {
        log_fatal("Failed to create shader module!");
        exit(1);
    }
}

void pipeline_destroy(RenderPipeline pipeline) {
    vkDestroyShaderModule(pipeline.device->vk_device, pipeline.frag_shader_module, NULL);
    vkDestroyShaderModule(pipeline.device->vk_device, pipeline.vert_shader_module, NULL);
    vkDestroyPipeline(pipeline.device->vk_device, pipeline.graphics_pipeline, NULL);
}