#ifndef TEXTURE_H
#define TEXTURE_H

#include "kore.h"
#include <vulkan/vulkan.h>

typedef struct TextureData {
    uint32 width, height;
    void *pixels;
} TextureData;

/* 
Creates a texture data object from an image file
    \param file The path to the image
    \return The texturedata object
*/
TextureData *texture_create_from_file(const char *file);

/*
Destroys and frees a texture data object. (It's important to do this, those texture datas are uncompressed and take a lot of memory)
    \param data The texture data to free
    */
void texture_data_destroy(TextureData *data);

typedef enum TextureFormat {
    /* Used for creating a color texture */
    TEXTURE_FORMAT_COLOR = VK_FORMAT_R8G8B8A8_UNORM,
    /* Used for creating a depth-stencil texture */
    TEXTURE_FORMAT_DEPTH_STENCIL = VK_FORMAT_D32_SFLOAT_S8_UINT,
    TEXTURE_FORMAT_COUNT,
} TextureFormat;

typedef enum TextureFilter
{
    /* Linearly interpolates sampling when the texture is scaled up */
    TEXTURE_FILTER_LINEAR = VK_FILTER_LINEAR,
    /* Chooses the nearest texel when the texture is scaled up */
    TEXTURE_FILTER_NEAREST = VK_FILTER_NEAREST,
    /* Not sure, haven't tested yet */
    TEXTURE_FILTER_CUBIC = VK_FILTER_CUBIC_IMG,
    TEXTURE_FILTER_COUNT,
} TextureFilter;

typedef enum TextureAddressMode
{
    /* Repeats texture sampling outside the uv bounds */
    TEXTURE_ADDRESS_MODE_REPEAT = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    /* Mirrored repeats sampling outside the uv bounds */
    TEXTURE_ADDRESS_MODE_MIRRORED_REPEAT = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    /* Clamps to one color outside the uv bounds */
    TEXTURE_ADDRESS_MODE_CLAMP = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    TEXTURE_ADDRESS_MODE_COUNT,
} TextureAddressMode;

typedef struct TextureConfigure
{
    /* The width of the texture to create. */
    /* Ignored if LoadFromData is true */
    uint32 wifth;
    /* The height of the texture to create. */
    /* Ignored if LoadFromData is true */
    uint32 height;
    /* The type of texture (color or depth-stencil). */
    /* Assumed to be color texture if LoadFromData is true */
    TextureFormat format;
    /* The sampling filter to use */
    TextureFilter filter;
    /* The address mode to use */
    TextureAddressMode address_mode;
    /* Whether or not to use anisotropic filtering */
    bool anisotropic_filtering;
    /* How many samplings to use for anisotropic filtering. Minimum is 1 and maximum is 16. */
    int32 anisotropy_level;
    /* Whether or not the texture is being loaded from data. */
    bool load_from_data;
    /* The texture data object used if LoadFromData is true */
    TextureData *data;
} TextureConfigure;

typedef struct Texture
{
    uint32 width, height;
    TextureFormat format;
    VkImage image;
    /*VmaAllocation Allocation; */
    VkImageView image_view;
    VkSampler sampler;
} *Texture;

/*
Creates a texture object from a configuration
    \param config The configuration to create from
    \return The texture object created
*/
Texture *texture_create(TextureConfigure config);

/* 
Places the texture into a queue to be destroyed. This should only be called if the texture needs to be destroyed at render-time
    \param texture The texture to destroy
*/
void texture_queue_destroy(Texture *texture);

/* 
Destroys and frees a texture object. Don't call this unless it's at the initialize or the deinitialize of the application, otherwise use TextureQueueDestroy
    \param texture The texture to destroy 
*/
void texture_destroy(Texture *texture);

#endif