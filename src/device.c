#include "device.h"
#include "log.h"

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

device device_init() {
    device d;
    d.physical_device = VK_NULL_HANDLE;
    
    create_instance(d);
    setup_debug_messenger(d);
    create_surface(d);
    pick_physical_device(d);
    create_logical_device(d);
    create_command_pool(d);
    
    return d;
}

void device_free(device dev) {
    vkDestroyCommandPool(dev.device_, dev.command_pool, NULL);
    vkDestroyDevice(dev.device_, NULL);
    
    if (enable_validation_layers == true) {
        DestroyDebugUtilsMessengerEXT(dev.instance, dev.debug_messenger, NULL);
    }
    
    vkDestroySurfaceKHR(dev.instance, dev.surface_, NULL);
    vkDestroyInstance(dev.instance, NULL);
}

void create_instance(device dev) {
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

    const char* extensions = get_required_extensions();
    create_info.enabledExtensionCount = NUM_ELEMENTS(extensions);
    create_info.ppEnabledExtensionNames = extensions;

    VkDebugUtilsMessengerCreateInfoEXT* debug_create_info;
    if (enable_validation_layers) {
        create_info.enabledLayerCount = NUM_ELEMENTS(validation_layers);
        create_info.ppEnabledLayerNames = validation_layers;

        populate_debug_messenger_create_info(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)debug_create_info;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = NULL;
    }

    if (vkCreateInstance(&create_info, NULL, dev.instance) != VK_SUCCESS) {
        log_error("Failed to create instance!");
    }

    has_glfw_required_instance_extensions();
}

void setup_debug_messenger(device dev) {
    if (!enable_validation_layers) return;
    VkDebugUtilsMessengerCreateInfoEXT create_info;
    populateDebugMessengerCreateInfo(create_info);
    if (CreateDebugUtilsMessengerEXT(dev.instance, create_info, NULL, dev.debug_messenger) != VK_SUCCESS) {
        log_error("Failed to set up debug messenger!");
    }
}

void create_surface(device dev) {
    window_create_surface(dev.window->glfw_window, dev.instance, dev.surface_);
}

void pick_physical_device(device dev) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(dev.instance, device_count, NULL);
    if (device_count == 0) {
        log_error("Failed to find GPUs with Vulkan support!");
    }
    log_info("Device count %d", device_count);
    VkPhysicalDevice* devices;
    vkEnumeratePhysicalDevices(dev.instance, device_count, devices);

    for (int i = 0; i < device_count; i++) {
        if (is_device_suitable(devices[i])) {
            dev.physical_device = devices[i];
            break;
        }
    }

    if (dev.physical_device == VK_NULL_HANDLE) {
        log_error("Failed to find a suitable GPU!");
    }

    vkGetPhysicalDeviceProperties(dev.physical_device, dev.properties);
    log_info("Physical Device: %c", dev.properties->deviceName);
}

void create_logical_device(device dev) {
    queue_family_indices indices = find_queue_families(dev.physical_device);

    VkDeviceQueueCreateInfo* queue_create_infos;
    uint32_t* unique_queue_families = { indices.graphics_family, indices.present_family };

    float queue_priority = 1.0f;
    for (uint32_t i = 0; i < unique_queue_families; i++) {
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = i;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos[i] = queue_create_info;
    }

    VkPhysicalDeviceFeatures* device_features;
    device_features->samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo* create_info;
    create_info->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info->queueCreateInfoCount = NUM_ELEMENTS(queue_create_infos);
    create_info->pQueueCreateInfos = queue_create_infos;

    create_info->pEnabledFeatures = device_features;
    create_info->enabledExtensionCount = NUM_ELEMENTS(device_extensions);
    create_info->ppEnabledExtensionNames = device_extensions;

    // might not really be necessary anymore because device specific validation layers
    // have been deprecated
    if (enable_validation_layers) {
        create_info->enabledLayerCount = NUM_ELEMENTS(validation_layers);
        create_info->ppEnabledLayerNames = validation_layers;
    } else {
        create_info->enabledLayerCount = 0;
    }

    if (vkCreateDevice(dev.physical_device, create_info, NULL, dev.device_) != VK_SUCCESS) {
        log_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(dev.device_, indices.graphics_family, 0, dev.graphicsQueue_);
    vkGetDeviceQueue(dev.device_, indices.present_family, 0, dev.presentQueue_);
}

void create_command_pool(device dev) {
    queue_family_indices qfi = find_queue_families(dev.physical_device);

    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = qfi.graphics_family;
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(dev.device_, &pool_info, NULL, &dev.command_pool) != VK_SUCCESS) {
        log_error("Failed to create command pool!");
    }
}

bool is_device_suitable(VkPhysicalDevice device) {
    queue_family_indices indices = find_queue_families(device);

    bool extensions_supported = check_device_extension_support(device);
    bool swap_chain_adequate = false;
    if (extensions_supported) {
        swap_chain_support_details swap_chain_support = query_swap_chain_support(device);
        swap_chain_adequate = swap_chain_support.formats != NULL && swap_chain_support.presentModes != NULL;
    }

    VkPhysicalDeviceFeatures* supported_features;
    vkGetPhysicalDeviceFeatures(device, supported_features);

    return is_queue_family_complete(indices) && extensions_supported && swap_chain_adequate && supported_features->samplerAnisotropy;
}


const char* get_required_extensions() {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    const char* extensions = *glfw_extensions;

    // YOU ARE HERE
    if (enable_validation_layers) {
        extensions[glfw_extension_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    return extensions;
}


bool check_validation_layer_support();
queue_family_indices find_queue_families(VkPhysicalDevice device);
void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
void has_glfw_required_instance_extensions();
bool check_device_extension_support(VkPhysicalDevice device);
swap_chain_support_details query_swap_chain_support(VkPhysicalDevice device);

















