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

typedef struct vk_device {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties* properties;
    VkCommandPool command_pool;
    window* window;

    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue compute_queue;
} vk_device;

static const char* validation_layers = { "VK_LAYER_KHRONOS_validation" };
static const uint32_t num_validation_layers = 1;

static const char* device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
static const uint32_t num_device_extensions = 1;

// device functions
vk_device vkd_init(window *w);
void vkd_free(vk_device dev);

// internal helper functions
void vkd_create_instance(vk_device dev);
void vkd_setup_debug_messenger(vk_device dev);
void vkd_create_surface(vk_device dev);
void vkd_pick_physical_device(vk_device dev, bool use_integrated);
void vkd_create_logical_device(vk_device dev);
void vkd_create_command_pool(vk_device dev);

uint32_t vkd_find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);
VkFormat vkd_find_supported_format(const VkFormat* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

// buffer helper functions
void vkd_create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* buffer_memory);
VkCommandBuffer vkd_begin_single_time_commands();
void vkd_end_single_time_commands(VkCommandBuffer command_buffer);
void vkd_copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
void vkd_copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layer_count);

void vkd_create_image_with_info(const VkImageCreateInfo* image_info, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* image_memory);

bool vkd_is_device_suitable(vk_device dev, VkPhysicalDevice device);
const char** vkd_get_required_extensions(uint32_t* pCount);
bool vkd_check_validation_layer_support();
bool vkd_is_queue_family_complete(queue_family_indices qf);
queue_family_indices vkd_find_queue_families(vk_device dev, VkPhysicalDevice device);
void vkd_populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
void vkd_has_glfw_required_instance_extensions();
bool vkd_check_device_extension_support(VkPhysicalDevice device);
swap_chain_support_details vkd_query_swap_chain_support(vk_device dev, VkPhysicalDevice device);

#endif