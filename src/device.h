#ifndef DEVICE_H
#define DEVICE_H

#include "window.h"

#define NUM_ELEMENTS(a) sizeof(a)/sizeof(*a)

typedef struct swap_chain_support_details {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
    VkSurfaceFormatKHR* presentModes;
} swap_chain_support_details;

typedef struct queue_family_indices {
    uint32_t graphics_family;
    uint32_t present_family;
    bool graphics_family_has_value;
    bool present_family_has_value;
} queue_family_indices;

bool is_queue_family_complete(queue_family_indices qf) { return qf.graphics_family_has_value && qf.present_family_has_value; }

#ifdef _DEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

typedef struct device {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties* properties;
    VkCommandPool command_pool;
    window* window;

    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;
} device;

const char* validation_layers = { "VK_LAYER_KHRONOS_validation" };
const char* device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

// device functions
device device_init();
void device_free(device dev);

// internal helper functions
void create_instance(device dev);
void setup_debug_messenger(device dev);
void create_surface(device dev);
void pick_physical_device(device dev);
void create_logical_device(device dev);
void create_command_pool(device dev);

uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);
VkFormat find_supported_format(const VkFormat* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

// buffer helper functions
void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* buffer_memory);
VkCommandBuffer begin_single_time_commands();
void end_single_time_commands(VkCommandBuffer command_buffer);
void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layer_count);

void create_image_with_info(const VkImageCreateInfo* image_info, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* image_memory);


bool is_device_suitable(VkPhysicalDevice device);
const char* get_required_extensions();
bool check_validation_layer_support();
queue_family_indices find_queue_families(VkPhysicalDevice device);
void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
void has_glfw_required_instance_extensions();
bool check_device_extension_support(VkPhysicalDevice device);
swap_chain_support_details query_swap_chain_support(VkPhysicalDevice device);

#endif