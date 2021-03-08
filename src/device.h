#ifndef DEVICE_H
#define DEVICE_H

#include "window.h"

#define NUM_ELEMENTS(a) sizeof(a)/sizeof(*a)

typedef struct swap_chain_support_details {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
    VkPresentModeKHR* presentModes;
} swap_chain_support_details;

typedef struct queue_family_indices {
    uint32_t graphics_family;
    uint32_t compute_family;
    uint32_t present_family;
    bool graphics_family_has_value;
    bool compute_family_has_value;
    bool present_family_has_value;
} queue_family_indices;

#ifdef _DEBUG
static const bool enable_validation_layers = true;
#else
static const bool enable_validation_layers = false;
#endif

typedef struct r_device {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties* properties;
    VkCommandPool command_pool;
    window* window;

    VkDevice vk_device;
    VkSurfaceKHR vk_surface;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue compute_queue;
} r_device;

static const char* validation_layers[] = { "VK_LAYER_KHRONOS_validation" };
static const uint32_t num_validation_layers = 1;

static const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
static const uint32_t num_device_extensions = 1;

/* device functions */
void device_init(window *w);
void device_free();

/* internal helper functions */
void device_create_instance();
void device_create_surface();
void device_pick_physical_device(bool use_integrated);
void device_create_logical_device();
void device_create_command_pool();

uint32_t device_find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);
VkFormat device_find_supported_format(const VkFormat* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

/* buffer helper functions */
void device_create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* buffer_memory);
VkCommandBuffer device_begin_single_time_commands();
void device_end_single_time_commands(VkCommandBuffer command_buffer);
void device_copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
void device_copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layer_count);

void device_create_image_with_info(const VkImageCreateInfo* image_info, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* image_memory);

bool device_is_device_suitable(VkPhysicalDevice device);
void device_get_required_extensions(const char **p_names, uint32_t* pCount);
bool device_check_validation_layer_support();
bool device_is_queue_family_complete(queue_family_indices qf);
queue_family_indices device_find_queue_families(VkPhysicalDevice device);
void device_has_glfw_required_instance_extensions();
bool device_check_device_extension_support(VkPhysicalDevice device);
swap_chain_support_details device_query_swap_chain_support(VkPhysicalDevice device);

#endif