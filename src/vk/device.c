#include "device.h"
#include "log.h"
#include "texture.h"
#include <stdio.h>
#include <string.h>

RenderDevice *device_create(Window *w) {
    RenderDevice *device;
    device = malloc(sizeof(*device));
    device->vk_physical_device = VK_NULL_HANDLE;
    device->window = w;

    log_info("---CHECKING EXTENSION SUPPORT---\n");
    device_check_instance_extension_support();
    
    log_info("---CREATING INSTANCE---\n");
    device_create_instance(device);

    log_info("---CREATING SURFACE---\n");
    device_create_surface(device);

    log_info("---PICKING PHYSICAL DEVICE---\n");
    device_pick_physical_device(device, false);

    log_info("---CREATING LOGICAL DEVICE---\n");
    device_create_logical_device(device);

    log_info("---CREATING RENDER PASS---\n");
    device_create_render_pass(device);

    log_info("---CREATING COMMAND POOL---\n");
    device_create_command_pool(device);

    return device;
}

void device_destroy(RenderDevice *device) {
    vkDestroyCommandPool(device->vk_device, device->command_pool, NULL);
    vkDestroyDevice(device->vk_device, NULL);
    
    vkDestroySurfaceKHR(device->instance, device->vk_surface, NULL);
    vkDestroyInstance(device->instance, NULL);

    free(device);
}

void device_create_instance(RenderDevice *device) {
    if (enable_validation_layers && !device_check_validation_layer_support()) {
        log_error("Validation layers requested, but not available!\n");
    }
    
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "rpg",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 0),
        .pEngineName = "Coolest Engine in da wurld!~~~",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    uint32 required_extensions_count;
    const char **required_extension_names = glfwGetRequiredInstanceExtensions(&required_extensions_count);
    
    VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = required_extensions_count,
        .ppEnabledExtensionNames = (const char *const *)required_extension_names,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .pNext = 0
    };

    if (enable_validation_layers) {
        create_info.enabledLayerCount = num_validation_layers;
        create_info.ppEnabledLayerNames = (const char *const *)validation_layers;
    }

    VkResult result = vkCreateInstance(&create_info, NULL, &device->instance);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkInstance: %i\n", result);
        exit(1);
    }
}

void device_create_surface(RenderDevice *device) {
    bool result = window_create_surface(device->instance, device->window->glfw_window, &device->vk_surface);

    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkSurfaceKHR\n");
        exit(1);
    }
}

void device_pick_physical_device(RenderDevice *device, bool use_integrated) {
    uint32 device_count = 0;
    vkEnumeratePhysicalDevices(device->instance, &device_count, NULL);
    if (device_count == 0) {
        log_fatal("Trying to initialize Vulkan, but there is no GPU with Vulkan support.\n");
        exit(1);
    }
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(device->instance, &device_count, devices);

    device->vk_physical_device = devices[0];

    uint32 i;
    for (i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(devices[i], &device_properties);

        if (device_is_device_suitable(device, devices[i])) {
            device->vk_physical_device = devices[i];
            if (!use_integrated && device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                break;
            }
            if (use_integrated && device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                break;
            }
        }
    }

    if (device->vk_physical_device == VK_NULL_HANDLE) {
        log_fatal("Trying to initialize Vulkan, but there is no suitable device found for Vulkan.\n");
        exit(1);
    }

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device->vk_physical_device, &device_properties);
    log_info("Using Graphics Device: %s\n", device_properties.deviceName);

    free(devices);
}

void device_create_logical_device(RenderDevice *device) {
    QueueFamilyIndices *indices = device_find_queue_families(device, device->vk_physical_device);

    float queue_priority = 1.0f;
    uint32 queue_count = 0;
    VkDeviceQueueCreateInfo queue_create_infos[3];

    VkDeviceQueueCreateInfo graphics_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indices->graphics_family,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    };
    queue_create_infos[queue_count] = graphics_queue_info;
    queue_count++;

    VkDeviceQueueCreateInfo present_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indices->present_family,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    };
    if (indices->present_family != indices->graphics_family) {
        queue_create_infos[queue_count] = present_queue_info;
        queue_count++;
    }

    if (indices->compute_family_has_value) {
        VkDeviceQueueCreateInfo compute_queue_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = indices->compute_family,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        };
        if (indices->compute_family != indices->graphics_family && indices->compute_family != indices->present_family)
        {
            queue_create_infos[queue_count] = compute_queue_info;
            queue_count++;
        }
    }

    VkPhysicalDeviceFeatures device_features = {
        .fillModeNonSolid = VK_true,
        .samplerAnisotropy = VK_true,
    };

    VkDeviceCreateInfo device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = queue_count,
        .pQueueCreateInfos = queue_create_infos,
        .pEnabledFeatures = &device_features,
        .enabledExtensionCount = num_device_extensions,
        .ppEnabledExtensionNames = device_extensions,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL
    };

    VkResult result = vkCreateDevice(device->vk_physical_device, &device_info, NULL, &device->vk_device);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize Vulkan, but failed to create VkDevice: %i\n", result);
        exit(1);
    }

    vkGetDeviceQueue(device->vk_device, indices->graphics_family, 0, &device->graphics_queue);
    vkGetDeviceQueue(device->vk_device, indices->present_family, 0, &device->present_queue);
    if (indices->compute_family_has_value) {
        vkGetDeviceQueue(device->vk_device, indices->compute_family, 0, &device->compute_queue);
    }

    free(indices);
}

void device_create_render_pass(RenderDevice *device) {
    VkAttachmentDescription color_attachment = {
        .format = (VkFormat)TEXTURE_FORMAT_COLOR,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout = VK_IMAGE_LAYOUT_GENERAL,
        .finalLayout = VK_IMAGE_LAYOUT_GENERAL,
    };
    VkAttachmentReference color_attachment_reference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    
    VkAttachmentDescription depth_attachment = {
        .format = (VkFormat)TEXTURE_FORMAT_DEPTH_STENCIL,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout = VK_IMAGE_LAYOUT_GENERAL,
        .finalLayout = VK_IMAGE_LAYOUT_GENERAL,
    };
    VkAttachmentReference depth_attachment_reference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    
    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment_reference,
        .pDepthStencilAttachment = &depth_attachment_reference,
    };
    
    VkAttachmentDescription attachments[] = { color_attachment, depth_attachment };
    VkRenderPassCreateInfo render_pass_info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 2,
        .pAttachments = attachments,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 0,
        .pDependencies = NULL,
    };
    VkResult result = vkCreateRenderPass(device->vk_device, &render_pass_info, NULL, &device->render_pass);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize RenderDevice, but failed to create VkRenderPass: %i\n", result);
        exit(1);
    }
}

void device_create_command_pool(RenderDevice *device) {
    QueueFamilyIndices *qfi = device_find_queue_families(device, device->vk_physical_device);

    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = qfi->graphics_family,
    };

    if (device->vk_device == NULL) {
        log_fatal("OH NO1!\n");
    }

    VkResult result = vkCreateCommandPool(device->vk_device, &pool_info, NULL, &device->command_pool);
    if (result != VK_SUCCESS) {
        log_fatal("Trying to initialize the RenderDevice, but failed to create VkCommandPool: %i\n", result);
        exit(1);
    }

    free(qfi);
}

bool device_is_device_suitable(RenderDevice *device, VkPhysicalDevice vk_physical_device) {
    QueueFamilyIndices *indices = device_find_queue_families(device, vk_physical_device);

    bool extensions_supported = device_check_device_extension_support(vk_physical_device);
    bool swap_chain_adequate = false;
    if (extensions_supported) {
        SwapChainSupportDetails swap_chain_support = device_query_swap_chain_support(device, vk_physical_device);
        swap_chain_adequate = swap_chain_support.formats != NULL && swap_chain_support.presentModes != NULL;
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(vk_physical_device, &supported_features);

    bool is_complete = device_is_queue_family_complete(indices);
    free(indices);
    return is_complete && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
}

const char **device_get_required_extensions(uint32* p_count) {
    uint32 glfw_extension_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    *p_count = glfw_extension_count + num_device_extensions;
    const char **p_names = (const char **)malloc(*p_count * sizeof(char *));
    uint32 i;
    for (i = 0; i < glfw_extension_count; i++) {
        p_names[i] = glfw_extensions[i];
    }
    for (i = 0; i < num_device_extensions; i++) {
        p_names[i + glfw_extension_count] = device_extensions[i];
    }

    return p_names;
}

bool device_check_validation_layer_support(void) {
    uint32 available_layer_count;
    vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

    VkLayerProperties* available_layers = malloc(available_layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers);

    bool supported = false;
    uint32 i;
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

bool device_is_queue_family_complete(QueueFamilyIndices *qf) {
    return qf->graphics_family_has_value && qf->present_family_has_value && qf->compute_family_has_value;
}

QueueFamilyIndices *device_find_queue_families(RenderDevice *device, VkPhysicalDevice vk_physical_device) {
    QueueFamilyIndices *indices;
    indices = malloc(sizeof(*indices));

    uint32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &queue_family_count, queue_families);

    uint32 i;
    for (i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices->graphics_family = i;
            indices->graphics_family_has_value = true;
        }

        if (queue_families[i].queueCount > 0 && queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices->compute_family = i;
            indices->compute_family_has_value = true;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vk_physical_device, i, device->vk_surface, &present_support);
        if (queue_families[i].queueCount > 0 && present_support) {
            indices->present_family = i;
            indices->present_family_has_value = true;
        }

        if (device_is_queue_family_complete(indices)) {
            break;
        }
    }

    free(queue_families);
    return indices;
}

void device_check_instance_extension_support() {
    unsigned int num_supported_extensions;
    vkEnumerateInstanceExtensionProperties(NULL, &num_supported_extensions, NULL);
    VkExtensionProperties *supported_extensions = malloc(num_supported_extensions * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &num_supported_extensions, supported_extensions);

    log_info("supported extensions: \n");
    uint32 ha;
    for (ha = 0; ha < num_supported_extensions; ha++) {
        log_info("\t%s\n", supported_extensions[ha]);
    }
    log_info("--------------------------------------\n");
    
    unsigned int num_required_extensions;
    const char **required_extension_names = glfwGetRequiredInstanceExtensions(&num_required_extensions);

    log_info("required extensions: \n");
    for (ha = 0; ha < num_required_extensions; ha++) {
        log_info("\t%s\n", required_extension_names[ha]);
    }
    log_info("--------------------------------------\n");
    
    uint32 i;
    for (i = 0; i < num_required_extensions; i++)
    {
        bool is_extension_supported = false;
        uint32 j;
        for (j = 0; j < num_supported_extensions; j++) {
            if (strcmp(supported_extensions[j].extensionName, required_extension_names[i]) == 0) {
                is_extension_supported = true;
                break;
            }
        }
        if (!is_extension_supported) {
            log_fatal("Trying to initialize Vulkan, but a required extension is not supported: %s\n", required_extension_names[i]);
            exit(1);
        }
    }
    free(supported_extensions);
}

bool device_check_device_extension_support(VkPhysicalDevice device) {
    uint32 supported_extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &supported_extension_count, NULL);
    VkExtensionProperties* supported_extensions = malloc(supported_extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &supported_extension_count, supported_extensions);

    uint32 required_extensions_count;
    const char **required_extension_names = device_get_required_extensions(&required_extensions_count);

    bool check = true;

    uint32 i;
    for (i = 0; i < required_extensions_count; i++) {
        bool extension_supported = false;
        uint32 j;
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

SwapChainSupportDetails device_query_swap_chain_support(RenderDevice *device, VkPhysicalDevice vk_physical_device) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physical_device, device->vk_surface, &details.capabilities);

    uint32 format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, device->vk_surface, &format_count, NULL);

    if (format_count != 0) {
        VkSurfaceFormatKHR* formats = (VkSurfaceFormatKHR*)malloc(format_count * sizeof(VkSurfaceFormatKHR));
        details.formats = formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, device->vk_surface, &format_count, details.formats);
        free(formats);
    }

    uint32 present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_device, device->vk_surface, &present_mode_count, NULL);

    if (present_mode_count != 0) {
        VkPresentModeKHR* modes = (VkPresentModeKHR*)malloc(format_count * sizeof(VkPresentModeKHR));
        details.presentModes = modes;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_device, device->vk_surface, &present_mode_count, details.presentModes);
    }
    return details;
}

















