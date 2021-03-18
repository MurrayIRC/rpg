#ifndef DEVICE_H
#define DEVICE_H

#include "core.h"
#include "window.h"

#define NUM_ELEMENTS(a) sizeof(a) / sizeof(*a)

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *presentModes;
} SwapChainSupportDetails;

typedef struct {
    uint32 graphics_family;
    uint32 compute_family;
    uint32 present_family;
    bool graphics_family_has_value;
    bool compute_family_has_value;
    bool present_family_has_value;
} QueueFamilyIndices;

#ifdef _DEBUG
static const bool enable_validation_layers = true;
#else
static const bool enable_validation_layers = false;
#endif

typedef struct {
    VkInstance instance;
    VkPhysicalDevice vk_physical_device;
    VkCommandPool command_pool;
    VkRenderPass render_pass;
    Window *window;

    VkDevice vk_device;
    VkSurfaceKHR vk_surface;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue compute_queue;
} RenderDevice;

static const char *validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
static const uint32 num_validation_layers = 1;

static const char *device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
static const uint32 num_device_extensions = 1;

/* device functions */
RenderDevice *device_create(Window *w);
void device_destroy(RenderDevice *device);

/* internal helper functions */
void device_create_instance(RenderDevice *device);
void device_create_surface(RenderDevice *device);
void device_pick_physical_device(RenderDevice *device, bool use_integrated);
void device_create_logical_device(RenderDevice *device);
void device_create_render_pass(RenderDevice *device);
void device_create_command_pool(RenderDevice *device);

uint32 device_find_memory_type(uint32 type_filter, VkMemoryPropertyFlags properties);
VkFormat device_find_supported_format(const VkFormat *candidates, VkImageTiling tiling,
                                      VkFormatFeatureFlags features);

/* buffer helper functions */
void device_create_buffer(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties, VkBuffer *buffer,
                          VkDeviceMemory *buffer_memory);
VkCommandBuffer device_begin_single_time_commands(void);
void device_end_single_time_commands(VkCommandBuffer command_buffer);
void device_copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
void device_copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32 width, uint32 height,
                                 uint32 layer_count);

void device_create_image_with_info(const VkImageCreateInfo *image_info,
                                   VkMemoryPropertyFlags properties, VkImage *image,
                                   VkDeviceMemory *image_memory);

bool device_is_device_suitable(RenderDevice *device, VkPhysicalDevice physical_device);
const char **device_get_required_extensions(uint32 *pCount);
bool device_check_validation_layer_support(void);
bool device_is_queue_family_complete(QueueFamilyIndices *qf);
QueueFamilyIndices *device_find_queue_families(RenderDevice *device,
                                               VkPhysicalDevice physical_device);
void device_check_instance_extension_support();
bool device_check_device_extension_support(VkPhysicalDevice device);
SwapChainSupportDetails device_query_swap_chain_support(RenderDevice *device,
                                                        VkPhysicalDevice physical_device);

#endif