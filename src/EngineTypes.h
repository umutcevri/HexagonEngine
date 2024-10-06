#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <functional>
#include <deque>
#include <iostream>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/packing.hpp>

#include "Camera.h"

#define ARRAY_MAX_TEXTURE_COUNT 256
#define MAX_OBJECT_COUNT 1000000

struct AllocatedBuffer {
    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo info;
};

struct Vertex {

    glm::vec3 position;
    float uv_x;
    glm::vec3 normal;
    float uv_y;
    glm::vec4 color;
    int textureID;
    int padding[3];
};

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 intensity;
};

// holds the resources needed for a mesh
struct GPUMeshBuffers {

    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    VkDeviceAddress vertexBufferAddress;
    
};

struct GPUMatrixBuffer
{
    AllocatedBuffer matrixBuffer;
    VkDeviceAddress matrixBufferAddress;
};

// push constants for our mesh object draws
struct GPUDrawPushConstants {
    glm::mat4 worldMatrix;
    VkDeviceAddress vertexBuffer;
    VkDeviceAddress matrixBuffer;
};

struct RenderObject
{
    GPUMeshBuffers buffers;
};

struct AllocatedImage {
    VkImage image;
    VkImageView imageView;
    VmaAllocation allocation;
    VkExtent3D imageExtent;
    VkFormat imageFormat;
    VkSampler sampler;
};


#define VK_CHECK(x)                                                     \
    do {                                                                \
        VkResult err = x;                                               \
        if (err) {                                                      \
            std::cout << "Detected Vulkan error: " << string_VkResult(err) << std::endl; \
            abort();                                                    \
        }                                                               \
    } while (0)
