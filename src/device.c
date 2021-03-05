#include "device.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    log_info("Validation Layer: %s", callback_data->pMessage);
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

vk_device device_init() {
    vk_device d;
    d.physical_device = VK_NULL_HANDLE;
    
    create_instance(d);
    setup_debug_messenger(d);
    create_surface(d);
    pick_physical_device(d, false);
    create_logical_device(d);
    create_command_pool(d);
    
    return d;
}

void device_free(vk_device dev) {
    vkDestroyCommandPool(dev.device_, dev.command_pool, NULL);
    vkDestroyDevice(dev.device_, NULL);
    
    if (enable_validation_layers == true) {
        DestroyDebugUtilsMessengerEXT(dev.instance, dev.debug_messenger, NULL);
    }
    
    vkDestroySurfaceKHR(dev.instance, dev.surface_, NULL);
    vkDestroyInstance(dev.instance, NULL);
}

void create_instance(vk_device dev) {
    if (enable_validation_layers && !check_validation_layer_support()) {
        log_error("Validation layers requested, but not available!");
    }
    
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "RPG App";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    uint32_t required_extensions_count;
    const char** extensions = get_required_extensions(&required_extensions_count);
    create_info.enabledExtensionCount = required_extensions_count;
    create_info.ppEnabledExtensionNames = (const char* const*)extensions;

    VkDebugUtilsMessengerCreateInfoEXT* debug_create_info;
    if (enable_validation_layers) {
        create_info.enabledLayerCount = num_validation_layers;
        create_info.ppEnabledLayerNames = &validation_layers;

        populate_debug_messenger_create_info(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)debug_create_info;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = NULL;
    }

    VkResult result = vkCreateInstance(&create_info, NULL, &dev.instance);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkInstance: %i\n", result);
        exit(1);
    }

    free(extensions);
    if (enable_validation_layers) {
        free(debug_create_info);
    }
    has_glfw_required_instance_extensions();
}

void setup_debug_messenger(vk_device dev) {
    if (!enable_validation_layers) return;
    VkDebugUtilsMessengerCreateInfoEXT* create_info;
    populate_debug_messenger_create_info(create_info);
    if (create_debug_utils_messenger_EXT(dev.instance, create_info, NULL, &dev.debug_messenger) != VK_SUCCESS) {
        log_error("Failed to set up debug messenger!");
    }

    free(create_info);
}

void create_surface(vk_device dev) {
    if (!window_create_surface(dev.window->glfw_window, dev.instance, &dev.surface_)) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkSurfaceKHR\n");
        exit(1);
    }
}

void pick_physical_device(vk_device dev, bool use_integrated) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(dev.instance, &device_count, NULL);
    if (device_count == 0) {
        log_fatal("Trying to initialize Vulkan, but there is no GPU with Vulkan support.\n");
        exit(1);
    }
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(dev.instance, &device_count, devices);

    dev.physical_device = devices[0];

    for (uint32_t i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(devices[i], &device_properties);

        if (is_device_suitable(dev, devices[i])) {
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

void create_logical_device(vk_device dev) {
    queue_family_indices indices = find_queue_families(dev, dev.physical_device);

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

    // might not really be necessary anymore because device specific validation layers
    // have been deprecated
    if (enable_validation_layers) {
        device_info.enabledLayerCount = num_validation_layers;
        device_info.ppEnabledLayerNames = (const char *const *)validation_layers;
    } else {
        device_info.enabledLayerCount = 0;
    }

    VkResult result = vkCreateDevice(dev.physical_device, &device_info, NULL, &dev.device_);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkDevice: %i\n", result);
        exit(1);
    }

    vkGetDeviceQueue(dev.device_, indices.graphics_family, 0, &dev.graphics_queue);
    vkGetDeviceQueue(dev.device_, indices.present_family, 0, &dev.present_queue);
    if (indices.compute_family_has_value) {
        vkGetDeviceQueue(dev.device_, indices.compute_family, 0, &dev.compute_queue);
    }
}

void create_command_pool(vk_device dev) {
    queue_family_indices qfi = find_queue_families(dev, dev.physical_device);

    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = qfi.graphics_family;
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(dev.device_, &pool_info, NULL, &dev.command_pool) != VK_SUCCESS) {
        log_error("Failed to create command pool!");
    }
}

bool is_device_suitable(vk_device dev, VkPhysicalDevice device) {
    queue_family_indices indices = find_queue_families(dev, device);

    bool extensions_supported = check_device_extension_support(device);
    bool swap_chain_adequate = false;
    if (extensions_supported) {
        swap_chain_support_details swap_chain_support = query_swap_chain_support(dev, device);
        swap_chain_adequate = swap_chain_support.formats != NULL && swap_chain_support.presentModes != NULL;
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(device, &supported_features);

    return is_queue_family_complete(indices) && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
}

const char** get_required_extensions(uint32_t* pCount) {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    uint32_t additional_extensions_count = sizeof(device_extensions) / sizeof(device_extensions[0]);
    uint32_t count = glfw_extension_count + additional_extensions_count;
    const char** names = malloc(sizeof(const char*) * count);
    for (uint32_t i = 0; i < glfw_extension_count; i++) {
        names[i] = glfw_extensions[i];
    }
    for (uint32_t i = 0; i < additional_extensions_count; i++) {
        names[i + glfw_extension_count] = &device_extensions[i];
    }

    if (enable_validation_layers) {
        count++;

        free(names);
        names = malloc(sizeof(const char*) * count);
        for (uint32_t i = 0; i < glfw_extension_count; i++) {
            names[i] = glfw_extensions[i];
        }
        for (uint32_t i = 0; i < additional_extensions_count; i++) {
            names[i + glfw_extension_count] = &device_extensions[i];
        }
        names[count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    pCount = &count;
    return names;
}


bool check_validation_layer_support() {
    uint32_t available_layer_count;
    vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

    VkLayerProperties* available_layers = malloc(available_layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers);

    bool supported = false;
    for (uint32_t i = 0; i < available_layer_count; i++) {
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

bool is_queue_family_complete(queue_family_indices qf) { 
    return qf.graphics_family_has_value && qf.present_family_has_value; 
}

queue_family_indices find_queue_families(vk_device dev, VkPhysicalDevice device) {
    queue_family_indices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && !indices.graphics_family_has_value) {
            indices.graphics_family = i;
            indices.graphics_family_has_value = true;
        }

        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT && !indices.compute_family_has_value) {
            indices.compute_family = i;
            indices.compute_family_has_value = true;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, dev.surface_, &present_support);
        if (queue_families[i].queueCount > 0 && present_support && !indices.present_family_has_value) {
            indices.present_family = i;
            indices.present_family_has_value = true;
        }

        if (is_queue_family_complete(indices)) {
            break;
        }
    }
    free(queue_families);
    return indices;
}

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* create_info) {
    VkDebugUtilsMessengerCreateInfoEXT* info = (VkDebugUtilsMessengerCreateInfoEXT*)malloc(sizeof(VkDebugUtilsMessengerCreateInfoEXT));
    info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    info->pfnUserCallback = debug_callback;
    info->pUserData = NULL; // optional
    create_info = info;
}

void has_glfw_required_instance_extensions() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
    VkExtensionProperties* extensions = malloc(extension_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

    printf("Available Extensions:\n");
    char* available[extension_count];

    for (uint32_t i = 0; i < extension_count; i++) {
        printf("\t%s\n", extensions[i].extensionName);
        available[i] = extensions[i].extensionName;
    }
    
    printf("Required Extensions:\n");
    uint32_t required_extensions_count;
    const char** required_extensions = get_required_extensions(&required_extensions_count);

    for (uint32_t i = 0; i < required_extensions_count; i++) {
        printf("\t%s", required_extensions[i]);
        bool found = false;
        for (uint32_t j = 0; j < extension_count; j++) {
            if (strcmp(available[j], required_extensions[i]) != 0) {
                found = true;
            }
        }

        if (!found) {
            log_error("Missing required GLFW extension.");
        }
    }

    free(required_extensions);
}

bool check_device_extension_support(VkPhysicalDevice device) {
    uint32_t supported_extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &supported_extension_count, NULL);
    VkExtensionProperties* supported_extensions = malloc(supported_extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &supported_extension_count, supported_extensions);

    uint32_t required_extensions_count;
    const char** required_extension_names = get_required_extensions(&required_extensions_count);

    bool check = true;

    for (uint32_t i = 0; i < required_extensions_count; i++) {
        bool extension_supported = false;
        for (uint32_t j = 0; j < supported_extension_count; j++) {
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

swap_chain_support_details query_swap_chain_support(vk_device dev, VkPhysicalDevice device) {
    swap_chain_support_details details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, dev.surface_, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, dev.surface_, &format_count, NULL);

    if (format_count != 0) {
        VkSurfaceFormatKHR* formats = (VkSurfaceFormatKHR*)malloc(format_count * sizeof(VkSurfaceFormatKHR));
        details.formats = formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, dev.surface_, &format_count, details.formats);
        free(formats);
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, dev.surface_, &present_mode_count, NULL);

    if (present_mode_count != 0) {
        VkPresentModeKHR* modes = (VkPresentModeKHR*)malloc(format_count * sizeof(VkPresentModeKHR));
        details.presentModes = modes;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, dev.surface_, &present_mode_count, details.presentModes);
    }
    return details;
}

















