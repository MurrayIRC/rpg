#include "device.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct r_device dev = { 0 };

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    log_info("Validation Layer: %s\n", callback_data->pMessage);
    return VK_FALSE;
}

VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    
    if (func != NULL) {
        return func(instance, create_info, allocator, debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
        func(instance, debug_messenger, allocator);
    }
}

void device_init(window *w) {
    dev.physical_device = VK_NULL_HANDLE;
    dev.window = w;
    
    log_info("CREATING INSTANCE\n");
    device_create_instance();

    log_info("CREATING SURFACE\n");
    device_create_surface();

    log_info("PICKING PHYSICAL DEVICE\n");
    device_pick_physical_device(false);

    log_info("CREATING LOGICAL DEVICE\n");
    device_create_logical_device();

    log_info("CREATING COMMAND POOL\n");
    device_create_command_pool();
}

void device_free() {
    vkDestroyCommandPool(dev.vk_device, dev.command_pool, NULL);
    vkDestroyDevice(dev.vk_device, NULL);
    
    if (enable_validation_layers == true) {
        DestroyDebugUtilsMessengerEXT(dev.instance, dev.debug_messenger, NULL);
    }
    
    vkDestroySurfaceKHR(dev.instance, dev.vk_surface, NULL);
    vkDestroyInstance(dev.instance, NULL);
}

void device_create_instance() {
    if (enable_validation_layers && !device_check_validation_layer_support()) {
        log_error("Validation layers requested, but not available!");
    }
    
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "rpg",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 0),
        .pEngineName = "Coolest Engine in da wurld!~~~",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    uint32_t required_extensions_count;
    device_get_required_extensions(NULL, &required_extensions_count);
    char **required_extension_names = (char **)malloc(required_extensions_count * sizeof(char *));
    device_get_required_extensions((const char **)required_extension_names, &required_extensions_count);
    
    VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = required_extensions_count,
        .ppEnabledExtensionNames = (const char* const*)required_extension_names,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
    };

    if (enable_validation_layers) {
        create_info.enabledLayerCount = num_validation_layers;
        create_info.ppEnabledLayerNames = (const char* const*)validation_layers;

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debug_callback,
            .pUserData = NULL
        };

        if (create_debug_utils_messenger_EXT(dev.instance, &debug_create_info, NULL, &dev.debug_messenger) != VK_SUCCESS) {
            log_error("Failed to set up debug messenger!\n");
        }

        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
    }

    VkResult result = vkCreateInstance(&create_info, NULL, &dev.instance);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkInstance: %i\n", result);
        exit(1);
    }

    free(required_extension_names);
    device_has_glfw_required_instance_extensions();
}

void device_create_surface() {
    if (!window_create_surface(dev.window->glfw_window, dev.instance, &dev.vk_surface)) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkSurfaceKHR\n");
        exit(1);
    }
}

void device_pick_physical_device(bool use_integrated) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(dev.instance, &device_count, NULL);
    if (device_count == 0) {
        log_fatal("Trying to initialize Vulkan, but there is no GPU with Vulkan support.\n");
        exit(1);
    }
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(dev.instance, &device_count, devices);

    dev.physical_device = devices[0];

    uint32_t i;
    for (i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(devices[i], &device_properties);

        if (device_is_device_suitable(devices[i])) {
            dev.physical_device = devices[i];
            if (!use_integrated && device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                break;
            }
            if (use_integrated && device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                break;
            }
        }
    }

    if (dev.physical_device == VK_NULL_HANDLE) {
        log_fatal("Trying to initialize Vulkan, but there is no suitable device found for Vulkan.\n");
        exit(1);
    }

    vkGetPhysicalDeviceProperties(dev.physical_device, dev.properties);
    log_info("Using Graphics Device: %s", dev.properties->deviceName);

    free(devices);
}

void device_create_logical_device() {
    queue_family_indices indices = device_find_queue_families(dev.physical_device);

    float queue_priority = 1.0f;
    uint32_t queue_count = 0;
    VkDeviceQueueCreateInfo queue_create_infos[3];

    VkDeviceQueueCreateInfo graphics_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indices.graphics_family,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    };
    queue_create_infos[queue_count] = graphics_queue_info;
    queue_count++;

    VkDeviceQueueCreateInfo present_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indices.present_family,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    };
    if (indices.present_family != indices.graphics_family) {
        queue_create_infos[queue_count] = present_queue_info;
        queue_count++;
    }

    if (indices.compute_family_has_value) {
        VkDeviceQueueCreateInfo compute_queue_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = indices.compute_family,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        };
        if (indices.compute_family != indices.graphics_family && indices.compute_family != indices.present_family)
        {
            queue_create_infos[queue_count] = compute_queue_info;
            queue_count++;
        }
    }

    VkPhysicalDeviceFeatures device_features = {
        .fillModeNonSolid = VK_TRUE,
        .samplerAnisotropy = VK_TRUE,
    };

    VkDeviceCreateInfo device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = queue_count,
        .pQueueCreateInfos = queue_create_infos,
        .pEnabledFeatures = &device_features,
        .enabledExtensionCount = num_device_extensions,
        .ppEnabledExtensionNames = (const char *const *)device_extensions,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL
    };

    /* might not really be necessary anymore because device specific validation layers have been deprecated */
    if (enable_validation_layers) {
        device_info.enabledLayerCount = num_validation_layers;
        device_info.ppEnabledLayerNames = (const char *const *)validation_layers;
    } else {
        device_info.enabledLayerCount = 0;
    }

    VkResult result = vkCreateDevice(dev.physical_device, &device_info, NULL, &dev.vk_device);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkDevice: %i\n", result);
        exit(1);
    }

    vkGetDeviceQueue(dev.vk_device, indices.graphics_family, 0, &dev.graphics_queue);
    vkGetDeviceQueue(dev.vk_device, indices.present_family, 0, &dev.present_queue);
    if (indices.compute_family_has_value) {
        vkGetDeviceQueue(dev.vk_device, indices.compute_family, 0, &dev.compute_queue);
    }
}

void device_create_command_pool() {
    queue_family_indices qfi = device_find_queue_families(dev.physical_device);

    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex = qfi.graphics_family,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    };

    if (vkCreateCommandPool(dev.vk_device, &pool_info, NULL, &dev.command_pool) != VK_SUCCESS) {
        log_error("Failed to create command pool!");
    }
}

bool device_is_device_suitable(VkPhysicalDevice device) {
    queue_family_indices indices = device_find_queue_families(device);

    bool extensions_supported = device_check_device_extension_support(device);
    bool swap_chain_adequate = false;
    if (extensions_supported) {
        swap_chain_support_details swap_chain_support = device_query_swap_chain_support(device);
        swap_chain_adequate = swap_chain_support.formats != NULL && swap_chain_support.presentModes != NULL;
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(device, &supported_features);

    return device_is_queue_family_complete(indices) && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
}

void device_get_required_extensions(const char **p_names, uint32_t* p_count) {
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    *p_count = glfw_extension_count + num_device_extensions;
    uint32_t i;
    for (i = 0; i < glfw_extension_count; i++) {
        if (p_names != NULL) {
            p_names[i] = glfw_extensions[i];
        }
    }
    for (i = 0; i < num_device_extensions; i++) {
        if (p_names != NULL) {
            p_names[i + glfw_extension_count] = device_extensions[i];
        }
    }

    if (enable_validation_layers) {
        *p_count += 1;

        for (i = 0; i < glfw_extension_count; i++) {
            if (p_names != NULL) {
                p_names[i] = glfw_extensions[i];
            }
        }
        for (i = 0; i < num_device_extensions - 1; i++) {
            if (p_names != NULL) {
                p_names[i + glfw_extension_count] = device_extensions[i];
            }
        }
        if (p_names != NULL) {
            p_names[*p_count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        }
    }
}


bool device_check_validation_layer_support() {
    uint32_t available_layer_count;
    vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

    VkLayerProperties* available_layers = malloc(available_layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers);

    bool supported = false;
    uint32_t i;
    for (i = 0; i < available_layer_count; i++) {
        if (strcmp(available_layers[i].layerName, "VK_LAYER_KHRONOS_validation") == 0) {
            supported = true;
        }
    }
    if (!supported) {
        log_warning("Validation layers not supported.\n");
    }

    free(available_layers);
    return supported;
}

bool device_is_queue_family_complete(queue_family_indices qf) { 
    return qf.graphics_family_has_value && qf.present_family_has_value; 
}

queue_family_indices device_find_queue_families(VkPhysicalDevice device) {
    queue_family_indices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    uint32_t i;
    for (i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && !indices.graphics_family_has_value) {
            indices.graphics_family = i;
            indices.graphics_family_has_value = true;
        }

        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT && !indices.compute_family_has_value) {
            indices.compute_family = i;
            indices.compute_family_has_value = true;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, dev.vk_surface, &present_support);
        if (queue_families[i].queueCount > 0 && present_support && !indices.present_family_has_value) {
            indices.present_family = i;
            indices.present_family_has_value = true;
        }

        if (device_is_queue_family_complete(indices)) {
            break;
        }
    }
    free(queue_families);
    return indices;
}

void device_has_glfw_required_instance_extensions() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
    VkExtensionProperties* extensions = malloc(extension_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

    log_info("Available Extensions:\n");
    char *available[extension_count];

    uint32_t i;
    for (i = 0; i < extension_count; i++) {
        log_info("\t%s\n", extensions[i].extensionName);
        available[i] = extensions[i].extensionName;
    }
    
    log_info("Required Extensions:\n");
    uint32_t required_extensions_count;
    device_get_required_extensions(NULL, &required_extensions_count);
    char **required_extension_names = (char **)malloc(required_extensions_count * sizeof(char *));
    device_get_required_extensions((const char **)required_extension_names, &extension_count);

    for (i = 0; i < required_extensions_count; i++) {
        log_info("\t%s\n", required_extension_names[i]);
        bool found = false;
        uint32_t j;
        for (j = 0; j < extension_count; j++) {
            if (strcmp(available[j], required_extension_names[i]) != 0) {
                found = true;
            }
        }

        if (!found) {
            log_error("Missing required GLFW extension.");
        }
    }

    free(required_extension_names);
}

bool device_check_device_extension_support(VkPhysicalDevice device) {
    uint32_t supported_extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &supported_extension_count, NULL);
    VkExtensionProperties* supported_extensions = malloc(supported_extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &supported_extension_count, supported_extensions);

    uint32_t required_extensions_count;
    device_get_required_extensions(NULL, &required_extensions_count);
    char **required_extension_names = (char **)malloc(required_extensions_count * sizeof(char*));
    device_get_required_extensions((const char **)required_extension_names, &required_extensions_count);

    bool check = true;

    uint32_t i;
    for (i = 0; i < required_extensions_count; i++) {
        bool extension_supported = false;
        uint32_t j;
        for (j = 0; j < supported_extension_count; j++) {
            if (strcmp(supported_extensions[j].extensionName, required_extension_names[i]) == 0) {
                extension_supported = true;
                break;
            }
        }

        if (!extension_supported) {
            check = false;
        }
    }

    free(supported_extensions);
    free(required_extension_names);

    return check;
}

swap_chain_support_details device_query_swap_chain_support(VkPhysicalDevice device) {
    swap_chain_support_details details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, dev.vk_surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, dev.vk_surface, &format_count, NULL);

    if (format_count != 0) {
        VkSurfaceFormatKHR* formats = (VkSurfaceFormatKHR*)malloc(format_count * sizeof(VkSurfaceFormatKHR));
        details.formats = formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, dev.vk_surface, &format_count, details.formats);
        free(formats);
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, dev.vk_surface, &present_mode_count, NULL);

    if (present_mode_count != 0) {
        VkPresentModeKHR* modes = (VkPresentModeKHR*)malloc(format_count * sizeof(VkPresentModeKHR));
        details.presentModes = modes;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, dev.vk_surface, &present_mode_count, details.presentModes);
    }
    return details;
}

















