#pragma once

#include "EngineTypes.h"
#include "EngineDescriptors.h"

#include <SDL/SDL.h>
#include <SDL/SDL_vulkan.h>

struct ComputePushConstants {
	glm::vec4 data1;
	glm::vec4 data2;
	glm::vec4 data3;
	glm::vec4 data4;
};

struct ComputeEffect {
	const char* name;

	VkPipeline pipeline;
	VkPipelineLayout layout;

	ComputePushConstants data;
};

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); //call functors
		}

		deletors.clear();
	}
};

struct FrameData {

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	VkFence _renderFence;

	DeletionQueue _deletionQueue;
	DescriptorAllocatorGrowable _frameDescriptors;

	AllocatedBuffer objectBuffer;
	VkDeviceAddress objectBufferAddress;

	AllocatedBuffer visibleInstanceBuffer;
	VkDeviceAddress visibleInstanceBufferAddress;

	AllocatedBuffer indirectCommandBuffer;
};

constexpr unsigned int FRAME_OVERLAP = 1;

class HexagonEngine {
public:
	Frustum frustum;

	bool bQuit = false;

	std::vector<Light> lights;

	std::vector<RenderObject> renderObjects;

	glm::mat4 view;
	glm::mat4 projection;
	float fov;

	float deltaTime;
	float lastFrame = 0;

	VkDescriptorSetLayout _singleImageDescriptorLayout;
	VkDescriptorSetLayout _cullingDescriptorLayout;

	std::vector<AllocatedImage> _images;

	AllocatedImage _depthImage;
	AllocatedImage _shadowDepthImage;

	VkSampler _defaultSamplerLinear;
	VkSampler _defaultSamplerNearest;

	AllocatedImage create_image(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
	AllocatedImage create_image(void* data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmapped = false);
	void destroy_image(const AllocatedImage& img);

	float renderScale = 1.f;
	bool resize_requested;

	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _meshPipeline;

	VkPipelineLayout _shadowPipelineLayout;
	VkPipeline _shadowPipeline;

	VkFence _immFence;
	VkCommandBuffer _immCommandBuffer;
	VkCommandPool _immCommandPool;

	std::vector<ComputeEffect> backgroundEffects;
	int currentBackgroundEffect{ 1 };

	VkPipeline _gradientPipeline;
	VkPipelineLayout _gradientPipelineLayout;

	VkPipeline _cullingPipeline;
	VkPipelineLayout _cullingPipelineLayout;


	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;

	AllocatedImage _drawImage;
	VkExtent2D _drawExtent;

	VmaAllocator _allocator;

	DeletionQueue _mainDeletionQueue;

	FrameData _frames[FRAME_OVERLAP];

	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	VkInstance _instance;// Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger;// Vulkan debug output handle
	VkPhysicalDevice _chosenGPU;// GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkSurfaceKHR _surface;// Vulkan window surface

	bool _isInitialized{ false };
	unsigned long long int _frameNumber{ 0 };
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1920 , 1080 };

	struct SDL_Window* _window{ nullptr };

	static HexagonEngine& Get();

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	void draw_background(VkCommandBuffer cmd);

	void draw_mesh(VkCommandBuffer cmd);

	void compute_culling(VkCommandBuffer cmd);

	void render();

	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();

	void create_mesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

	void renderObjectBufferDelete();

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	void SetViewMatrix(glm::mat4 matrix);

	void SetFOV(float _fov);

	ObjectBufferData* getObjectBufferData();

private:
	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();

	void init_descriptors();

	void init_pipelines();
	void init_background_pipelines();

	void init_imgui();

	void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);

	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	void destroy_buffer(const AllocatedBuffer& buffer);

	GPUMeshBuffers uploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

	void init_mesh_pipeline();

	void init_shadow_pipeline();

	void init_culling_pipeline();

	void resize_swapchain();

	AllocatedImage loadTexture(const char* texturePath);

	void init_sampler();

	void draw_shadows(VkCommandBuffer cmd);

	


};
