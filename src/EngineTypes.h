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
#include <map>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/packing.hpp>

#define ARRAY_MAX_TEXTURE_COUNT 256
#define MAX_OBJECT_COUNT 2000000
#define WORKGROUP_SIZE 64

struct Plane
{
    // unit vector
    glm::vec3 normal = { 0.f, 1.f, 0.f };

    // distance from origin to the nearest point in the plane
    float distance = 0.f;

    Plane() {}
    Plane(const glm::vec3& p1, const glm::vec3& norm) : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)){}
};

struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;

    Frustum() {}
};

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

struct Object
{
    glm::mat4 renderMatrix;
    glm::vec3 color;
    bool isSolidColor = false;
    glm::vec2 chunkID;
    glm::vec3 blockCoords;
};

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 intensity;
};

struct GPUMeshBuffers {

    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    VkDeviceAddress vertexBufferAddress;
    
};

struct GPUDrawPushConstants {
    glm::mat4 worldMatrix;
    VkDeviceAddress vertexBuffer;
    VkDeviceAddress objectBuffer;
    VkDeviceAddress visibleInstanceBuffer;
};

struct CullingPushConstants {
    Plane frustumPlanes[6];
    VkDeviceAddress visibleInstanceBuffer;
    VkDeviceAddress objectBuffer;
};

struct RenderObject
{
    GPUMeshBuffers buffers;
    std::vector<Object> instances;
    int indexCount;
    int instanceCount;

    RenderObject(GPUMeshBuffers b, int indexcount) : buffers(b), indexCount(indexcount){}
};

struct ObjectBufferData
{
    glm::mat4 renderMatrix;
    glm::vec3 color;
    int isSolidColor;
    glm::vec3 boundingSphereCenter;
    float boundingSphereRadius;
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
