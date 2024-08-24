////////////////////////////////////////////////////////////////////////////////
// Copyright 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License.  You may obtain a copy
// of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
////////////////////////////////////////////////////////////////////////////////
///DRE 2024 - ADDED CONFIGURE
#ifndef HAVE_ALARM
#include "config.h"
#endif

#if defined(VK_USE_GLFW_NUKLEAR)

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <vector>

//#include "demoshaders/overlay.frag.h"
//#include "demoshaders/overlay.vert.h"
//#include "demoshaders/triangle.frag.h"
//#include "demoshaders/triangle.vert.h"
#include "overlay.h"
#endif

#if !defined(TUTORIAL_07_HEADER)
#define TUTORIAL_07_HEADER

#include "VulkanCommon.h"
#include "Tools.h"

namespace ApiWithoutSecrets
{

// ************************************************************ //
// VertexData                                                   //
//                                                              //
// Struct describing data type and format of vertex attributes  //
// ************************************************************ //
struct VertexData
{
    float   x, y, z, w;
    float   u, v;
};

// ************************************************************ //
// RenderingResourcesData                                       //
//                                                              //
// Struct containing data used during rendering process         //
// ************************************************************ //
struct RenderingResourcesData
{
    VkFramebuffer                         Framebuffer;
    VkCommandBuffer                       CommandBuffer;
    VkSemaphore                           ImageAvailableSemaphore;
    VkSemaphore                           FinishedRenderingSemaphore;
    VkFence                               Fence;

    RenderingResourcesData() :
        Framebuffer( VK_NULL_HANDLE ),
        CommandBuffer( VK_NULL_HANDLE ),
        ImageAvailableSemaphore( VK_NULL_HANDLE ),
        FinishedRenderingSemaphore( VK_NULL_HANDLE ),
        Fence( VK_NULL_HANDLE )
    {
    }
};

// ************************************************************ //
// VulkanTutorial04Parameters                                   //
//                                                              //
// Vulkan specific parameters                                   //
// ************************************************************ //
struct VulkanTutorial07Parameters
{
    VkRenderPass                          RenderPass;
    ImageParameters                       Image;
    BufferParameters                      UniformBuffer;
    DescriptorSetParameters               DescriptorSet;
    VkPipelineLayout                      PipelineLayout;
    VkPipeline                            GraphicsPipeline;
    BufferParameters                      VertexBuffer;
    BufferParameters                      StagingBuffer;
    VkCommandPool                         CommandPool;
    std::vector<RenderingResourcesData>   RenderingResources;

    static const size_t                   ResourcesCount = 3;




    VulkanTutorial07Parameters() :
        RenderPass( VK_NULL_HANDLE ),
        Image(),
        UniformBuffer(),
        DescriptorSet(),
        PipelineLayout(),
        GraphicsPipeline( VK_NULL_HANDLE ),
        VertexBuffer(),
        StagingBuffer(),
        CommandPool( VK_NULL_HANDLE ),
        RenderingResources( ResourcesCount )
    {
    }
};

// ************************************************************ //
// Tutorial04                                                   //
//                                                              //
// Class for presenting Vulkan usage topics                     //
// ************************************************************ //
class Tutorial07 : public VulkanCommon
{
//! ---- UNCHANGED MEMBERS FROM DEMO.CPP
//! ADDED DRE 2024
    VkInstance instance;
    VkDebugReportCallbackEXT callback;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    std::vector<VkImage> overlayImages;
    std::vector<VkImageView> overlayImageViews;
    std::vector<VkDeviceMemory> overlayImageMemories;

    VkRenderPass renderPass;
    //VkPipelineLayout trianglePipelineLayout;
    //VkPipeline triangleGraphicsPipeline;
    VkPipelineLayout overlayPipelineLayout;
    VkPipeline overlayGraphicsPipeline;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

    overlay_settings settings;

//! ADDED DRE 2024	
	
public:
    Tutorial07();
    ~Tutorial07();

    bool    CreateRenderingResources();
    bool    CreateStagingBuffer();
    bool    CreateTexture();
    bool    CreateUniformBuffer();
    bool    CreateDescriptorSetLayout();
    bool    CreateDescriptorPool();
    bool    AllocateDescriptorSet();
    bool    UpdateDescriptorSet();
    bool    CreateRenderPass();
    bool    CreatePipelineLayout();
    bool    CreatePipeline();
    bool    CreateVertexBuffer();

    bool    Draw() override;

private:
    VulkanTutorial07Parameters  Vulkan;

    bool                                                              CreateCommandBuffers();
    bool                                                              CreateCommandPool(uint32_t queue_family_index, VkCommandPool *pool);
    bool                                                              AllocateCommandBuffers(VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers);
    bool                                                              CreateSemaphores();
    bool                                                              CreateFences();
    bool                                                              CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlagBits memoryProperty, BufferParameters &buffer);
    bool                                                              AllocateBufferMemory(VkBuffer buffer, VkMemoryPropertyFlagBits property, VkDeviceMemory *memory);
    bool                                                              CreateImage( uint32_t width, uint32_t height, VkImage *image );
    bool                                                              AllocateImageMemory( VkImage image, VkMemoryPropertyFlagBits property, VkDeviceMemory *memory );
    bool                                                              CreateImageView( ImageParameters &image_parameters );
    bool                                                              CreateSampler( VkSampler *sampler );
    bool                                                              CopyTextureData( char *texture_data, uint32_t data_size, uint32_t width, uint32_t height );
    const std::array<float, 16>                                       GetUniformBufferData() const;
    bool                                                              CopyUniformBufferData();
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>     CreateShaderModule( const char* filename );
    const std::vector<float>&                                         GetVertexData() const;
    bool                                                              CopyVertexData();
    bool                                                              PrepareFrame( VkCommandBuffer command_buffer, const ImageParameters &image_parameters, VkFramebuffer &framebuffer );
    bool                                                              CreateFramebuffer( VkFramebuffer &framebuffer, VkImageView image_view );
    void                                                              DestroyBuffer( BufferParameters& buffer );

    bool                                                              ChildOnWindowSizeChanged() override;
    void                                                              ChildClear() override;
};

} // namespace ApiWithoutSecrets

#endif // TUTORIAL_07_HEADER
