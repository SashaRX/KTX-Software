/* -*- tab-width: 4; -*- */
/* vi: set sw=2 ts=4 expandtab textwidth=80: */

/*
 * Copyright 2025 Khronos Group, Inc.
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file ktx_emscripten_helpers.c
 * @brief Helper functions for Emscripten builds to access ktxTexture2 members
 *
 * These functions are necessary because Emscripten/WASM cannot directly access
 * C struct members. They must be exported via EMSCRIPTEN_KEEPALIVE and called
 * through cwrap() from JavaScript.
 */

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <ktx.h>
#include <stdint.h>

/**
 * @brief Get texture data pointer
 * @param texture Pointer to ktxTexture2
 * @return Pointer to texture data
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint8_t* ktx_get_data(ktxTexture2* texture) {
    if (texture == NULL) return NULL;
    return texture->pData;
}

/**
 * @brief Get texture data size
 * @param texture Pointer to ktxTexture2
 * @return Size of texture data in bytes
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
size_t ktx_get_data_size(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->dataSize;
}

/**
 * @brief Get base width of texture
 * @param texture Pointer to ktxTexture2
 * @return Width of level 0 in pixels
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_base_width(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->baseWidth;
}

/**
 * @brief Get base height of texture
 * @param texture Pointer to ktxTexture2
 * @return Height of level 0 in pixels
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_base_height(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->baseHeight;
}

/**
 * @brief Get base depth of texture
 * @param texture Pointer to ktxTexture2
 * @return Depth of level 0 in pixels (1 for 2D textures)
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_base_depth(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->baseDepth;
}

/**
 * @brief Get number of mipmap levels
 * @param texture Pointer to ktxTexture2
 * @return Number of mipmap levels
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_num_levels(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->numLevels;
}

/**
 * @brief Get number of array layers
 * @param texture Pointer to ktxTexture2
 * @return Number of array layers
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_num_layers(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->numLayers;
}

/**
 * @brief Get number of faces (6 for cubemaps, 1 otherwise)
 * @param texture Pointer to ktxTexture2
 * @return Number of faces
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_num_faces(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->numFaces;
}

/**
 * @brief Get number of dimensions (1, 2, or 3)
 * @param texture Pointer to ktxTexture2
 * @return Number of dimensions
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_num_dimensions(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->numDimensions;
}

/**
 * @brief Check if texture is an array
 * @param texture Pointer to ktxTexture2
 * @return 1 if array, 0 otherwise
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_is_array(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->isArray ? 1 : 0;
}

/**
 * @brief Check if texture is a cubemap
 * @param texture Pointer to ktxTexture2
 * @return 1 if cubemap, 0 otherwise
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_is_cubemap(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->isCubemap ? 1 : 0;
}

/**
 * @brief Check if texture is compressed
 * @param texture Pointer to ktxTexture2
 * @return 1 if compressed, 0 otherwise
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_is_compressed(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->isCompressed ? 1 : 0;
}

/**
 * @brief Get Vulkan format
 * @param texture Pointer to ktxTexture2
 * @return VkFormat value
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_vk_format(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->vkFormat;
}

/**
 * @brief Get supercompression scheme
 * @param texture Pointer to ktxTexture2
 * @return Supercompression scheme
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
uint32_t ktx_get_supercompression_scheme(ktxTexture2* texture) {
    if (texture == NULL) return 0;
    return texture->supercompressionScheme;
}

/**
 * @brief Get image offset within texture data
 * @param texture Pointer to ktxTexture2
 * @param level Mipmap level
 * @param layer Array layer
 * @param faceSlice Face index for cubemaps
 * @param pOffset Pointer to receive the offset
 * @return KTX_SUCCESS or error code
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
ktx_error_code_e ktx_get_image_offset(ktxTexture2* texture,
                                       uint32_t level,
                                       uint32_t layer,
                                       uint32_t faceSlice,
                                       ktx_size_t* pOffset) {
    if (texture == NULL || pOffset == NULL) {
        return KTX_INVALID_VALUE;
    }
    return ktxTexture_GetImageOffset(ktxTexture(texture), level, layer, faceSlice, pOffset);
}

/**
 * @brief Get image size for a specific mipmap level
 * @param texture Pointer to ktxTexture2
 * @param level Mipmap level
 * @return Size of the image in bytes
 */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
ktx_size_t ktx_get_image_size(ktxTexture2* texture, uint32_t level) {
    if (texture == NULL) return 0;
    return ktxTexture_GetImageSize(ktxTexture(texture), level);
}
