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

#include "Tutorial07.h"
#include "VulkanFunctions.h"

namespace ApiWithoutSecrets
{

const std::vector<const char *> validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char *> deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugReportCallbackEXT( VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(  instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr( instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete()
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
//!---- UNCHANGED MEMBER FUNCTIONS
    void createSurface()
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void createInstance()
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
        {
            throw std::runtime_error( "validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void setupDebugCallback()
    {
        if (!enableValidationLayers)
            return;

        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = debugCallback;

        if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up debug callback!");
        }
    }

    void pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto &device : devices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void createLogicalDevice()
    {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = {indices.graphicsFamily,
                                             indices.presentFamily
                                            };

        float queuePriority = 1.0f;
        for (int queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
    }

    void createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        uint32_t queueFamilyIndices[] = {(uint32_t)indices.graphicsFamily,
                                         (uint32_t)indices.presentFamily
                                        };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }
    
    void createSwapChainImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
			/// Provided by VK_VERSION_1_0
			//VkResult vkCreateImageView(
			//VkDevice device,
			//const VkImageViewCreateInfo* pCreateInfo,
			//const VkAllocationCallbacks* pAllocator,
			///VkImageView* pView);
            if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }     
   void createOverlayImages()
    {
		/// resizes overlayimages to the same size as the swapchain
        overlayImages.resize(swapChainImages.size());
        /// resizes imageoverlaymemories resized to swapchain
        overlayImageMemories.resize(swapChainImages.size());
        // for every overlayimages
        for (size_t i = 0; i < overlayImages.size(); i++)
        {
            VkImageCreateInfo imageInfo = {};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExtent.width;
            imageInfo.extent.height = swapChainExtent.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = swapChainImageFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			/// Provided by VK_VERSION_1_0
			//VkResult vkCreateImage(
			//VkDevice device,
			//const VkImageCreateInfo* pCreateInfo,
			//const VkAllocationCallbacks* pAllocator,
			//VkImage* pImage);
           if (vkCreateImage(device, &imageInfo, nullptr, &overlayImages[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image!");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(device, overlayImages[i], &memRequirements);
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;

            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
            VkMemoryPropertyFlags properties;
            bool found = false;
            uint32_t j;
            for (j = 0; j < memProperties.memoryTypeCount; j++)
            {
                if ((memRequirements.memoryTypeBits & (1 << j)) && (memProperties.memoryTypes[j].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                throw std::runtime_error("failed to find suitable memory type!");
            }
            // select the last iterated value j
            allocInfo.memoryTypeIndex = j;
			/// Provided by VK_VERSION_1_0
			//VkResult vkAllocateMemory(
			//VkDevice device,
			//const VkMemoryAllocateInfo* pAllocateInfo,
			//const VkAllocationCallbacks* pAllocator,
			///VkDeviceMemory* pMemory); 
           if (vkAllocateMemory(device, &allocInfo, nullptr, &overlayImageMemories[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate image memory!");
            }
            vkBindImageMemory(device, overlayImages[i], overlayImageMemories[i], 0);
        }// end for every overlayimages
    }// end  createOverlayImages    


    void createOverlayImageViews()
    {
        overlayImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < overlayImageViews.size(); i++)
        {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = overlayImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &createInfo, nullptr, &overlayImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
        }
    } // end createOverlayImageViews

    void createRenderPass()
    {
        VkAttachmentDescription attachments[2] = {};
        attachments[0].format = swapChainImageFormat;
        attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        attachments[1].format = swapChainImageFormat;
        attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[1].initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        attachments[1].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference inputAttachmentRef = {};
        inputAttachmentRef.attachment = 1;
        inputAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkSubpassDescription subpasses[2] = {};
        subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpasses[0].colorAttachmentCount = 1;
        subpasses[0].pColorAttachments = &colorAttachmentRef;

        subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpasses[1].colorAttachmentCount = 1;
        subpasses[1].pColorAttachments = &colorAttachmentRef;
        subpasses[1].inputAttachmentCount = 1;
        subpasses[1].pInputAttachments = &inputAttachmentRef;

        VkSubpassDependency dependencies[2] = {};
        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[0].srcAccessMask = 0;
        dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        dependencies[1].srcSubpass = 0;
        dependencies[1].dstSubpass = 1;
        dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[1].srcAccessMask = 0;
        dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 2;
        renderPassInfo.pAttachments = attachments;
        renderPassInfo.subpassCount = 2;
        renderPassInfo.pSubpasses = subpasses;
        renderPassInfo.dependencyCount = 2;
        renderPassInfo.pDependencies = dependencies;

        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    void createFramebuffers()
    {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++)
        {
            VkImageView attachments[2] = {swapChainImageViews[i], overlayImageViews[i] };
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 2;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
    void createDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding overlayLayoutBinding = {};
        overlayLayoutBinding.binding = 0;
        overlayLayoutBinding.descriptorCount = 1;
        overlayLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        overlayLayoutBinding.pImmutableSamplers = nullptr;
        overlayLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = 1;
        descriptorSetLayoutCreateInfo.pBindings = &overlayLayoutBinding;

        if (vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create post descriptor set layout!");
        }
    }

    void createDescriptorPool()
    {
        VkDescriptorPoolSize poolSize = {};
        poolSize.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        poolSize.descriptorCount = static_cast<uint32_t>(swapChainFramebuffers.size());

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(swapChainFramebuffers.size());
        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
    
    void createDescriptorSets()
    {
        uint32_t numFbs = swapChainFramebuffers.size();
        descriptorSets.resize(numFbs, nullptr);

        std::vector<VkDescriptorSetLayout> layouts(numFbs, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = numFbs;
        allocInfo.pSetLayouts = layouts.data();
        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocated descriptor sets!");
        }

        for (size_t i = 0; i < numFbs; i++)
        {
            VkDescriptorImageInfo descriptorImageInfo = {};
            descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            descriptorImageInfo.imageView = overlayImageViews[i];

            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pImageInfo = &descriptorImageInfo;

            vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
        }
    }        

    VkShaderModule createShaderModule(const std::vector<char> &code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }// end createShaderModule

    void createOverlayGraphicsPipeline()
    {
        std::vector<char> vertShaderCode(
            &demoshaders_overlay_vert_spv[0],
            &demoshaders_overlay_vert_spv[demoshaders_overlay_vert_spv_len]);
        std::vector<char> fragShaderCode(
            &demoshaders_overlay_frag_spv[0],
            &demoshaders_overlay_frag_spv[demoshaders_overlay_frag_spv_len]);

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                          fragShaderStageInfo
                                                         };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 1.0f;
        colorBlending.blendConstants[1] = 1.0f;
        colorBlending.blendConstants[2] = 1.0f;
        colorBlending.blendConstants[3] = 1.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &overlayPipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = overlayPipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &overlayGraphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, fragShaderModule, nullptr);
        vkDestroyShaderModule(device, vertShaderModule, nullptr);
    }// end createOverlayGraphicsPipeline
    
    void createCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }// end createCommandPool

    void createCommandBuffers()
    {
        commandBuffers.resize(swapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

            vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChainExtent;

            VkClearValue clearColor = {settings.bg_color[0], settings.bg_color[1], settings.bg_color[2], settings.bg_color[3] };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, triangleGraphicsPipeline);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
            vkCmdNextSubpass(commandBuffers[i], VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, overlayGraphicsPipeline);
            vkCmdBindDescriptorSets( commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, overlayPipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }// end createCommandBuffers
    
    void createSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||  vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphores!");
        }
    }// end createSemaphores

   void drawFrame()
    {
		// drawing to this image index in images
		uint32_t imageIndex;
		///	// Provided by VK_KHR_swapchain - acquires image index to write into
		//	VkResult vkAcquireNextImageKHR(
		//    VkDevice                                    device,
		//    VkSwapchainKHR                              swapchain,
		//    uint64_t                                    timeout,
		//    VkSemaphore                                 semaphore,
		//    VkFence                                     fence,
		//    uint32_t*                                   pImageIndex);
        VkResult result = vkAcquireNextImageKHR( device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        float old_bg_color[4] {settings.bg_color[0], settings.bg_color[1], settings.bg_color[2], settings.bg_color[3] };
        // overlay.c function to execute overlay drawing
		//VkSemaphore submit_overlay(struct overlay_settings *settings,
		//	   VkQueue graphics_queue, uint32_t buffer_index,
		//	   VkSemaphore wait_semaphore)    
		/// submitted without a wait semaphore    
        VkSemaphore overlayFinished = submit_overlay(&settings, graphicsQueue, imageIndex, nullptr);
		/// Provided by VK_VERSION_1_0
		//		typedef struct VkSubmitInfo 
		//		//{
		//		VkStructureType	sType;
		//		const void* pNext;
		//		uint32_t waitSemaphoreCount;
		//		const VkSemaphore* pWaitSemaphores;
		//		const VkPipelineStageFlags* pWaitDstStageMask;
		//		uint32_t commandBufferCount;
		//		const VkCommandBuffer* pCommandBuffers;
		//		uint32_t signalSemaphoreCount;
		//		const VkSemaphore* pSignalSemaphores;
		//		//} VkSubmitInfo;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore, overlayFinished};
        VkPipelineStageFlags waitStages[] =
        {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        submitInfo.waitSemaphoreCount = 2;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
		/// Provided by VK_VERSION_1_0
		//VkResult vkQueueSubmit(
		//VkQueue  queue,
		//uint32_t submitCount,
		//const VkSubmitInfo* pSubmits,
		///VkFence fence);
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        if (old_bg_color[0] != settings.bg_color[0] || old_bg_color[1] != settings.bg_color[1] || old_bg_color[2] != settings.bg_color[2] || old_bg_color[3] != settings.bg_color[3])
        {
            // update clear color
            createCommandBuffers();
        }
///		// Provided by VK_KHR_swapchain
//		typedef struct VkPresentInfoKHR {
//			VkStructureType  sType;
//			const void* pNext;
//			uint32_t waitSemaphoreCount;
//			const VkSemaphore* pWaitSemaphores;
//			uint32_t swapchainCount;
//			const VkSwapchainKHR* pSwapchains;
//			const uint32_t* pImageIndices;
//			VkResult* pResults;
//		} VkPresentInfoKHR;
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
		/// a handle of a swapchain that the image will be bound to.
        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;
		/// Provided by VK_KHR_swapchain
		//VkResult vkQueuePresentKHR(
		//VkQueue  queue,
		///const VkPresentInfoKHR* pPresentInfo);
        result = vkQueuePresentKHR(presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }
		///vkQueueWaitIdle is equivalent to having submitted a valid fence to every previously executed queue
		///submission command that accepts a fence, then waiting for all of those fences to signal using
		///vkWaitForFences with an infinite timeout and waitAll set to VK_TRUE.
        vkQueueWaitIdle(presentQueue);
    }// end drawFrame
       
//!---- UNCHANGED MEMBER FUNCTIONS
//!---- UNCHANGED MEMBER FUNCTIONS

//!----
	//! Vulkan doesn't hold onto state other than struct sent in so you can substitute without repercussions...
    void initVulkan()
    {
		// the goal is translate and remove unnneeded functions using all the previously found data into same configurations
		/// removed not needed
		// changed to data setting
        createInstance();
        ///setupDebugCallback();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createSwapChainImageViews();
        createOverlayImages();
        createOverlayImageViews();
        createRenderPass();
        createFramebuffers();
        createDescriptorSetLayout();
        createDescriptorPool();
        createDescriptorSets();
        //createTriangleGraphicsPipeline();
        createOverlayGraphicsPipeline();
        createCommandPool();
        createCommandBuffers();
        createSemaphores();

        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

        init_overlay(
            window, device, physicalDevice, queueFamilyIndices.graphicsFamily,
            graphicsQueue, overlayImageViews.data(), overlayImageViews.size(),
            swapChainExtent.width, swapChainExtent.height, swapChainImageFormat);
    }

Tutorial07::Tutorial07()
{
}

bool Tutorial07::CreateRenderingResources()
{
    if( !CreateCommandBuffers() )
    {
        return false;
    }
    if( !CreateSemaphores() )
    {
        return false;
    }
    if( !CreateFences() )
    {
        return false;
    }
    //! This is the overlay insert into process
    if( !initVulkan()() )
    {
        return false;
    }
    return true;
}

//! \fn CreateCommandBuffers  creates Vulkan.CommandPool and then based on swapchain size creates Vulkan.RenderingResources[i].CommandBuffer s linked to Vulkan.CommandPool
bool Tutorial07::CreateCommandBuffers()
{
    if( !CreateCommandPool( GetGraphicsQueue().FamilyIndex, &Vulkan.CommandPool ) )
    {
        std::cout << "Could not create command pool!" << std::endl;
        return false;
    }
    for( size_t i = 0; i < Vulkan.RenderingResources.size(); ++i )
    {
        if( !AllocateCommandBuffers( Vulkan.CommandPool, 1, &Vulkan.RenderingResources[i].CommandBuffer ) )
        {
            std::cout << "Could not allocate command buffer!" << std::endl;
            return false;
        }
    }
    return true;
}

//! \fn CreateCommandPool \var queue_family_index from queue family and output to \var pool and returns TRUE or FALSE
bool Tutorial07::CreateCommandPool( uint32_t queue_family_index, VkCommandPool *pool )
{
    VkCommandPoolCreateInfo cmd_pool_create_info =
    {
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,       // VkStructureType                sType
        nullptr,                                          // const void                    *pNext
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | // VkCommandPoolCreateFlags       flags
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        queue_family_index                                // uint32_t                       queueFamilyIndex
    };

    if( vkCreateCommandPool( GetDevice(), &cmd_pool_create_info, nullptr, pool ) != VK_SUCCESS )
    {
        return false;
    }
    return true;
}

bool Tutorial07::AllocateCommandBuffers( VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers )
{
    VkCommandBufferAllocateInfo command_buffer_allocate_info =
    {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,   // VkStructureType                sType
        nullptr,                                          // const void                    *pNext
        pool,                                             // VkCommandPool                  commandPool
        VK_COMMAND_BUFFER_LEVEL_PRIMARY,                  // VkCommandBufferLevel           level
        count                                             // uint32_t                       bufferCount
    };

    if( vkAllocateCommandBuffers( GetDevice(), &command_buffer_allocate_info, command_buffers ) != VK_SUCCESS )
    {
        return false;
    }
    return true;
}

/// \fn CreateSemaphores - creates a semaphore on a per-RenderingResourcesData basis called \var RenderingResources[i].ImageAvailableSemaphore and \var RenderingResources[i].FinishedRenderingSemaphore
bool Tutorial07::CreateSemaphores()
{
    VkSemaphoreCreateInfo semaphore_create_info =
    {
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,      // VkStructureType          sType
        nullptr,                                      // const void*              pNext
        0                                             // VkSemaphoreCreateFlags   flags
    };

    for( size_t i = 0; i < Vulkan.RenderingResources.size(); ++i )
    {
        if( (vkCreateSemaphore( GetDevice(), &semaphore_create_info, nullptr, &Vulkan.RenderingResources[i].ImageAvailableSemaphore ) != VK_SUCCESS) || (vkCreateSemaphore( GetDevice(), &semaphore_create_info, nullptr, &Vulkan.RenderingResources[i].FinishedRenderingSemaphore ) != VK_SUCCESS) )
        {
            std::cout << "Could not create semaphores!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Tutorial07::CreateFences()
{
    VkFenceCreateInfo fence_create_info =
    {
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,              // VkStructureType                sType
        nullptr,                                          // const void                    *pNext
        VK_FENCE_CREATE_SIGNALED_BIT                      // VkFenceCreateFlags             flags
    };

    for( size_t i = 0; i < Vulkan.RenderingResources.size(); ++i )
    {
        if( vkCreateFence( GetDevice(), &fence_create_info, nullptr, &Vulkan.RenderingResources[i].Fence ) != VK_SUCCESS )
        {
            std::cout << "Could not create a fence!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Tutorial07::CreateStagingBuffer()
{
    Vulkan.StagingBuffer.Size = 1000000;
    if( !CreateBuffer( VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, Vulkan.StagingBuffer ) )
    {
        std::cout << "Could not create staging buffer!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::CreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlagBits memoryProperty, BufferParameters &buffer )
{
    VkBufferCreateInfo buffer_create_info =
    {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,             // VkStructureType        sType
        nullptr,                                          // const void            *pNext
        0,                                                // VkBufferCreateFlags    flags
        buffer.Size,                                      // VkDeviceSize           size
        usage,                                            // VkBufferUsageFlags     usage
        VK_SHARING_MODE_EXCLUSIVE,                        // VkSharingMode          sharingMode
        0,                                                // uint32_t               queueFamilyIndexCount
        nullptr                                           // const uint32_t        *pQueueFamilyIndices
    };

    if( vkCreateBuffer( GetDevice(), &buffer_create_info, nullptr, &buffer.Handle ) != VK_SUCCESS )
    {
        std::cout << "Could not create buffer!" << std::endl;
        return false;
    }

    if( !AllocateBufferMemory( buffer.Handle, memoryProperty, &buffer.Memory ) )
    {
        std::cout << "Could not allocate memory for a buffer!" << std::endl;
        return false;
    }

    if( vkBindBufferMemory( GetDevice(), buffer.Handle, buffer.Memory, 0 ) != VK_SUCCESS )
    {
        std::cout << "Could not bind memory to a buffer!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::AllocateBufferMemory( VkBuffer buffer, VkMemoryPropertyFlagBits property, VkDeviceMemory *memory )
{
    VkMemoryRequirements buffer_memory_requirements;
    vkGetBufferMemoryRequirements( GetDevice(), buffer, &buffer_memory_requirements );

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties( GetPhysicalDevice(), &memory_properties );

    for( uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i )
    {
        if( (buffer_memory_requirements.memoryTypeBits & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & property) )
        {

            VkMemoryAllocateInfo memory_allocate_info =
            {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,     // VkStructureType                        sType
                nullptr,                                    // const void                            *pNext
                buffer_memory_requirements.size,            // VkDeviceSize                           allocationSize
                i                                           // uint32_t                               memoryTypeIndex
            };

            if( vkAllocateMemory( GetDevice(), &memory_allocate_info, nullptr, memory ) == VK_SUCCESS )
            {
                return true;
            }
        }
    }
    return false;
}

bool Tutorial07::CreateTexture()
{
    int width = 0, height = 0, data_size = 0;
    // DRE 2024 - changed location to find .png
    //std::vector<char> texture_data = Tools::GetImageData( "Data/Tutorials/07/texture.png", 4, &width, &height, nullptr, &data_size );
    std::vector<char> texture_data = Tools::GetImageData( "Data/texture.png", 4, &width, &height, nullptr, &data_size );
    if( texture_data.size() == 0 )
    {
        return false;
    }

    if( !CreateImage( width, height, &Vulkan.Image.Handle ) )
    {
        std::cout << "Could not create image!" << std::endl;
        return false;
    }

    if( !AllocateImageMemory( Vulkan.Image.Handle, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &Vulkan.Image.Memory ) )
    {
        std::cout << "Could not allocate memory for image!" << std::endl;
        return false;
    }

    if( vkBindImageMemory( GetDevice(), Vulkan.Image.Handle, Vulkan.Image.Memory, 0 ) != VK_SUCCESS )
    {
        std::cout << "Could not bind memory to an image!" << std::endl;
        return false;
    }

    if( !CreateImageView( Vulkan.Image ) )
    {
        std::cout << "Could not create image view!" << std::endl;
        return false;
    }

    if( !CreateSampler( &Vulkan.Image.Sampler ) )
    {
        std::cout << "Could not create sampler!" << std::endl;
        return false;
    }

    if( !CopyTextureData( texture_data.data(), data_size, width, height ) )
    {
        std::cout << "Could not upload texture data to device memory!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::CreateImage( uint32_t width, uint32_t height, VkImage *image )
{
    VkImageCreateInfo image_create_info =
    {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,                  // VkStructureType            sType;
        nullptr,                                              // const void                *pNext
        0,                                                    // VkImageCreateFlags         flags
        VK_IMAGE_TYPE_2D,                                     // VkImageType                imageType
        VK_FORMAT_R8G8B8A8_UNORM,                             // VkFormat                   format
        {
            // VkExtent3D                 extent
            width,                                                // uint32_t                   width
            height,                                               // uint32_t                   height
            1                                                     // uint32_t                   depth
        },
        1,                                                    // uint32_t                   mipLevels
        1,                                                    // uint32_t                   arrayLayers
        VK_SAMPLE_COUNT_1_BIT,                                // VkSampleCountFlagBits      samples
        VK_IMAGE_TILING_OPTIMAL,                              // VkImageTiling              tiling
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |                     // VkImageUsageFlags          usage
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_SHARING_MODE_EXCLUSIVE,                            // VkSharingMode              sharingMode
        0,                                                    // uint32_t                   queueFamilyIndexCount
        nullptr,                                              // const uint32_t*            pQueueFamilyIndices
        VK_IMAGE_LAYOUT_UNDEFINED                             // VkImageLayout              initialLayout
    };

    return vkCreateImage( GetDevice(), &image_create_info, nullptr, image ) == VK_SUCCESS;
}

bool Tutorial07::AllocateImageMemory( VkImage image, VkMemoryPropertyFlagBits property, VkDeviceMemory *memory )
{
    VkMemoryRequirements image_memory_requirements;
    vkGetImageMemoryRequirements( GetDevice(), image, &image_memory_requirements );

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties( GetPhysicalDevice(), &memory_properties );

    for( uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i )
    {
        if( (image_memory_requirements.memoryTypeBits & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & property) )
        {

            VkMemoryAllocateInfo memory_allocate_info =
            {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,     // VkStructureType                        sType
                nullptr,                                    // const void                            *pNext
                image_memory_requirements.size,             // VkDeviceSize                           allocationSize
                i                                           // uint32_t                               memoryTypeIndex
            };

            if( vkAllocateMemory( GetDevice(), &memory_allocate_info, nullptr, memory ) == VK_SUCCESS )
            {
                return true;
            }
        }
    }
    return false;
}

bool Tutorial07::CreateImageView( ImageParameters &image_parameters )
{
    VkImageViewCreateInfo image_view_create_info =
    {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,             // VkStructureType            sType
        nullptr,                                              // const void                *pNext
        0,                                                    // VkImageViewCreateFlags     flags
        image_parameters.Handle,                              // VkImage                    image
        VK_IMAGE_VIEW_TYPE_2D,                                // VkImageViewType            viewType
        VK_FORMAT_R8G8B8A8_UNORM,                             // VkFormat                   format
        {
            // VkComponentMapping         components
            VK_COMPONENT_SWIZZLE_IDENTITY,                        // VkComponentSwizzle         r
            VK_COMPONENT_SWIZZLE_IDENTITY,                        // VkComponentSwizzle         g
            VK_COMPONENT_SWIZZLE_IDENTITY,                        // VkComponentSwizzle         b
            VK_COMPONENT_SWIZZLE_IDENTITY                         // VkComponentSwizzle         a
        },
        {
            // VkImageSubresourceRange    subresourceRange
            VK_IMAGE_ASPECT_COLOR_BIT,                            // VkImageAspectFlags         aspectMask
            0,                                                    // uint32_t                   baseMipLevel
            1,                                                    // uint32_t                   levelCount
            0,                                                    // uint32_t                   baseArrayLayer
            1                                                     // uint32_t                   layerCount
        }
    };

    return vkCreateImageView( GetDevice(), &image_view_create_info, nullptr, &image_parameters.View ) == VK_SUCCESS;
}

bool Tutorial07::CreateSampler( VkSampler *sampler )
{
    VkSamplerCreateInfo sampler_create_info =
    {
        VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,                // VkStructureType            sType
        nullptr,                                              // const void*                pNext
        0,                                                    // VkSamplerCreateFlags       flags
        VK_FILTER_LINEAR,                                     // VkFilter                   magFilter
        VK_FILTER_LINEAR,                                     // VkFilter                   minFilter
        VK_SAMPLER_MIPMAP_MODE_NEAREST,                       // VkSamplerMipmapMode        mipmapMode
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,                // VkSamplerAddressMode       addressModeU
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,                // VkSamplerAddressMode       addressModeV
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,                // VkSamplerAddressMode       addressModeW
        0.0f,                                                 // float                      mipLodBias
        VK_FALSE,                                             // VkBool32                   anisotropyEnable
        1.0f,                                                 // float                      maxAnisotropy
        VK_FALSE,                                             // VkBool32                   compareEnable
        VK_COMPARE_OP_ALWAYS,                                 // VkCompareOp                compareOp
        0.0f,                                                 // float                      minLod
        0.0f,                                                 // float                      maxLod
        VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,              // VkBorderColor              borderColor
        VK_FALSE                                              // VkBool32                   unnormalizedCoordinates
    };

    return vkCreateSampler( GetDevice(), &sampler_create_info, nullptr, sampler ) == VK_SUCCESS;
}

bool Tutorial07::CopyTextureData( char *texture_data, uint32_t data_size, uint32_t width, uint32_t height )
{
    // Prepare data in staging buffer

    void *staging_buffer_memory_pointer;
    if( vkMapMemory( GetDevice(), Vulkan.StagingBuffer.Memory, 0, data_size, 0, &staging_buffer_memory_pointer ) != VK_SUCCESS )
    {
        std::cout << "Could not map memory and upload texture data to a staging buffer!" << std::endl;
        return false;
    }

    memcpy( staging_buffer_memory_pointer, texture_data, data_size );

    VkMappedMemoryRange flush_range =
    {
        VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,              // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        Vulkan.StagingBuffer.Memory,                        // VkDeviceMemory                         memory
        0,                                                  // VkDeviceSize                           offset
        data_size                                           // VkDeviceSize                           size
    };
    vkFlushMappedMemoryRanges( GetDevice(), 1, &flush_range );

    vkUnmapMemory( GetDevice(), Vulkan.StagingBuffer.Memory );

    // Prepare command buffer to copy data from staging buffer to a vertex buffer
    VkCommandBufferBeginInfo command_buffer_begin_info =
    {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,        // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,        // VkCommandBufferUsageFlags              flags
        nullptr                                             // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    VkCommandBuffer command_buffer = Vulkan.RenderingResources[0].CommandBuffer;

    vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info);

    VkImageSubresourceRange image_subresource_range =
    {
        VK_IMAGE_ASPECT_COLOR_BIT,                          // VkImageAspectFlags                     aspectMask
        0,                                                  // uint32_t                               baseMipLevel
        1,                                                  // uint32_t                               levelCount
        0,                                                  // uint32_t                               baseArrayLayer
        1                                                   // uint32_t                               layerCount
    };

    VkImageMemoryBarrier image_memory_barrier_from_undefined_to_transfer_dst =
    {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,             // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        0,                                                  // VkAccessFlags                          srcAccessMask
        VK_ACCESS_TRANSFER_WRITE_BIT,                       // VkAccessFlags                          dstAccessMask
        VK_IMAGE_LAYOUT_UNDEFINED,                          // VkImageLayout                          oldLayout
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,               // VkImageLayout                          newLayout
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               dstQueueFamilyIndex
        Vulkan.Image.Handle,                                // VkImage                                image
        image_subresource_range                             // VkImageSubresourceRange                subresourceRange
    };
    vkCmdPipelineBarrier( command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier_from_undefined_to_transfer_dst);

    VkBufferImageCopy buffer_image_copy_info =
    {
        0,                                                  // VkDeviceSize                           bufferOffset
        0,                                                  // uint32_t                               bufferRowLength
        0,                                                  // uint32_t                               bufferImageHeight
        {
            // VkImageSubresourceLayers               imageSubresource
            VK_IMAGE_ASPECT_COLOR_BIT,                          // VkImageAspectFlags                     aspectMask
            0,                                                  // uint32_t                               mipLevel
            0,                                                  // uint32_t                               baseArrayLayer
            1                                                   // uint32_t                               layerCount
        },
        {
            // VkOffset3D                             imageOffset
            0,                                                  // int32_t                                x
            0,                                                  // int32_t                                y
            0                                                   // int32_t                                z
        },
        {
            // VkExtent3D                             imageExtent
            width,                                              // uint32_t                               width
            height,                                             // uint32_t                               height
            1                                                   // uint32_t                               depth
        }
    };
    vkCmdCopyBufferToImage( command_buffer, Vulkan.StagingBuffer.Handle, Vulkan.Image.Handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy_info );

    VkImageMemoryBarrier image_memory_barrier_from_transfer_to_shader_read =
    {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,             // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_ACCESS_TRANSFER_WRITE_BIT,                       // VkAccessFlags                          srcAccessMask
        VK_ACCESS_SHADER_READ_BIT,                          // VkAccessFlags                          dstAccessMask
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,               // VkImageLayout                          oldLayout
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,           // VkImageLayout                          newLayout
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               dstQueueFamilyIndex
        Vulkan.Image.Handle,                                // VkImage                                image
        image_subresource_range                             // VkImageSubresourceRange                subresourceRange
    };
    vkCmdPipelineBarrier( command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier_from_transfer_to_shader_read);

    vkEndCommandBuffer( command_buffer );

    // Submit command buffer and copy data from staging buffer to a vertex buffer
    VkSubmitInfo submit_info =
    {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,                      // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        0,                                                  // uint32_t                               waitSemaphoreCount
        nullptr,                                            // const VkSemaphore                     *pWaitSemaphores
        nullptr,                                            // const VkPipelineStageFlags            *pWaitDstStageMask;
        1,                                                  // uint32_t                               commandBufferCount
        &command_buffer,                                    // const VkCommandBuffer                 *pCommandBuffers
        0,                                                  // uint32_t                               signalSemaphoreCount
        nullptr                                             // const VkSemaphore                     *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_info, VK_NULL_HANDLE ) != VK_SUCCESS )
    {
        return false;
    }

    vkDeviceWaitIdle( GetDevice() );

    return true;
}

bool Tutorial07::CreateUniformBuffer()
{
    Vulkan.UniformBuffer.Size = 16 * sizeof(float);
    if( !CreateBuffer( VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Vulkan.UniformBuffer ) )
    {
        std::cout << "Could not create uniform buffer!" << std::endl;
        return false;
    }

    if( !CopyUniformBufferData() )
    {
        return false;
    }

    return true;
}

const std::array<float, 16> Tutorial07::GetUniformBufferData() const
{
    float half_width = static_cast<float>(GetSwapChain().Extent.width) * 0.5f;
    float half_height = static_cast<float>(GetSwapChain().Extent.height) * 0.5f;

    return Tools::GetOrthographicProjectionMatrix( -half_width, half_width, -half_height, half_height, -1.0f, 1.0f );
}

bool Tutorial07::CopyUniformBufferData()
{
    // Prepare data in staging buffer
    const std::array<float, 16> uniform_data = GetUniformBufferData();

    void *staging_buffer_memory_pointer;
    if( vkMapMemory( GetDevice(), Vulkan.StagingBuffer.Memory, 0, Vulkan.UniformBuffer.Size, 0, &staging_buffer_memory_pointer) != VK_SUCCESS )
    {
        std::cout << "Could not map memory and upload data to a staging buffer!" << std::endl;
        return false;
    }

    memcpy( staging_buffer_memory_pointer, uniform_data.data(), Vulkan.UniformBuffer.Size );

    VkMappedMemoryRange flush_range =
    {
        VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,  // VkStructureType  sType
        nullptr,                                // const void      *pNext
        Vulkan.StagingBuffer.Memory,            // VkDeviceMemory   memory
        0,                                      // VkDeviceSize     offset
        Vulkan.UniformBuffer.Size               // VkDeviceSize     size
    };
    vkFlushMappedMemoryRanges( GetDevice(), 1, &flush_range );

    vkUnmapMemory( GetDevice(), Vulkan.StagingBuffer.Memory );

    // Prepare command buffer to copy data from staging buffer to a uniform buffer
    VkCommandBuffer command_buffer = Vulkan.RenderingResources[0].CommandBuffer;

    VkCommandBufferBeginInfo command_buffer_begin_info =
    {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,        // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,        // VkCommandBufferUsageFlags              flags
        nullptr                                             // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info);

    VkBufferCopy buffer_copy_info =
    {
        0,                                                  // VkDeviceSize                           srcOffset
        0,                                                  // VkDeviceSize                           dstOffset
        Vulkan.UniformBuffer.Size                           // VkDeviceSize                           size
    };
    vkCmdCopyBuffer( command_buffer, Vulkan.StagingBuffer.Handle, Vulkan.UniformBuffer.Handle, 1, &buffer_copy_info );

    VkBufferMemoryBarrier buffer_memory_barrier =
    {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,            // VkStructureType                        sType;
        nullptr,                                            // const void                            *pNext
        VK_ACCESS_TRANSFER_WRITE_BIT,                       // VkAccessFlags                          srcAccessMask
        VK_ACCESS_UNIFORM_READ_BIT,                         // VkAccessFlags                          dstAccessMask
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               dstQueueFamilyIndex
        Vulkan.UniformBuffer.Handle,                        // VkBuffer                               buffer
        0,                                                  // VkDeviceSize                           offset
        VK_WHOLE_SIZE                                       // VkDeviceSize                           size
    };
    vkCmdPipelineBarrier( command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, 0, 0, nullptr, 1, &buffer_memory_barrier, 0, nullptr );

    vkEndCommandBuffer( command_buffer );

    // Submit command buffer and copy data from staging buffer to a vertex buffer
    VkSubmitInfo submit_info =
    {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,                      // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        0,                                                  // uint32_t                               waitSemaphoreCount
        nullptr,                                            // const VkSemaphore                     *pWaitSemaphores
        nullptr,                                            // const VkPipelineStageFlags            *pWaitDstStageMask;
        1,                                                  // uint32_t                               commandBufferCount
        &command_buffer,                                    // const VkCommandBuffer                 *pCommandBuffers
        0,                                                  // uint32_t                               signalSemaphoreCount
        nullptr                                             // const VkSemaphore                     *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_info, VK_NULL_HANDLE ) != VK_SUCCESS )
    {
        return false;
    }

    vkDeviceWaitIdle( GetDevice() );
    return true;
}

bool Tutorial07::CreateDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> layout_bindings =
    {
        {
            0,                                                  // uint32_t                             binding
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,          // VkDescriptorType                     descriptorType
            1,                                                  // uint32_t                             descriptorCount
            VK_SHADER_STAGE_FRAGMENT_BIT,                       // VkShaderStageFlags                   stageFlags
            nullptr                                             // const VkSampler                     *pImmutableSamplers
        },
        {
            1,                                                  // uint32_t                             binding
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                  // VkDescriptorType                     descriptorType
            1,                                                  // uint32_t                             descriptorCount
            VK_SHADER_STAGE_VERTEX_BIT,                         // VkShaderStageFlags                   stageFlags
            nullptr                                             // const VkSampler                     *pImmutableSamplers
        }
    };

    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info =
    {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,  // VkStructureType                      sType
        nullptr,                                              // const void                          *pNext
        0,                                                    // VkDescriptorSetLayoutCreateFlags     flags
        static_cast<uint32_t>(layout_bindings.size()),        // uint32_t                             bindingCount
        layout_bindings.data()                                // const VkDescriptorSetLayoutBinding  *pBindings
    };

    if( vkCreateDescriptorSetLayout( GetDevice(), &descriptor_set_layout_create_info, nullptr, &Vulkan.DescriptorSet.Layout ) != VK_SUCCESS )
    {
        std::cout << "Could not create descriptor set layout!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::CreateDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> pool_sizes =
    {
        {
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,          // VkDescriptorType                     type
            1                                                   // uint32_t                             descriptorCount
        },
        {
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                  // VkDescriptorType                     type
            1                                                   // uint32_t                             descriptorCount
        }
    };

    VkDescriptorPoolCreateInfo descriptor_pool_create_info =
    {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,        // VkStructureType                      sType
        nullptr,                                              // const void                          *pNext
        0,                                                    // VkDescriptorPoolCreateFlags          flags
        1,                                                    // uint32_t                             maxSets
        static_cast<uint32_t>(pool_sizes.size()),             // uint32_t                             poolSizeCount
        pool_sizes.data()                                     // const VkDescriptorPoolSize          *pPoolSizes
    };

    if( vkCreateDescriptorPool( GetDevice(), &descriptor_pool_create_info, nullptr, &Vulkan.DescriptorSet.Pool ) != VK_SUCCESS )
    {
        std::cout << "Could not create descriptor pool!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::AllocateDescriptorSet()
{
    VkDescriptorSetAllocateInfo descriptor_set_allocate_info =
    {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, // VkStructureType                sType
        nullptr,                                        // const void                    *pNext
        Vulkan.DescriptorSet.Pool,                      // VkDescriptorPool               descriptorPool
        1,                                              // uint32_t                       descriptorSetCount
        &Vulkan.DescriptorSet.Layout                    // const VkDescriptorSetLayout   *pSetLayouts
    };

    if( vkAllocateDescriptorSets( GetDevice(), &descriptor_set_allocate_info, &Vulkan.DescriptorSet.Handle ) != VK_SUCCESS )
    {
        std::cout << "Could not allocate descriptor set!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::UpdateDescriptorSet()
{
    VkDescriptorImageInfo image_info =
    {
        Vulkan.Image.Sampler,                           // VkSampler                      sampler
        Vulkan.Image.View,                              // VkImageView                    imageView
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL        // VkImageLayout                  imageLayout
    };

    VkDescriptorBufferInfo buffer_info =
    {
        Vulkan.UniformBuffer.Handle,                    // VkBuffer                       buffer
        0,                                              // VkDeviceSize                   offset
        Vulkan.UniformBuffer.Size                       // VkDeviceSize                   range
    };

    std::vector<VkWriteDescriptorSet> descriptor_writes =
    {
        {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,       // VkStructureType                sType
            nullptr,                                      // const void                    *pNext
            Vulkan.DescriptorSet.Handle,                  // VkDescriptorSet                dstSet
            0,                                            // uint32_t                       dstBinding
            0,                                            // uint32_t                       dstArrayElement
            1,                                            // uint32_t                       descriptorCount
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,    // VkDescriptorType               descriptorType
            &image_info,                                  // const VkDescriptorImageInfo   *pImageInfo
            nullptr,                                      // const VkDescriptorBufferInfo  *pBufferInfo
            nullptr                                       // const VkBufferView            *pTexelBufferView
        },
        {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,       // VkStructureType                sType
            nullptr,                                      // const void                    *pNext
            Vulkan.DescriptorSet.Handle,                  // VkDescriptorSet                dstSet
            1,                                            // uint32_t                       dstBinding
            0,                                            // uint32_t                       dstArrayElement
            1,                                            // uint32_t                       descriptorCount
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,            // VkDescriptorType               descriptorType
            nullptr,                                      // const VkDescriptorImageInfo   *pImageInfo
            &buffer_info,                                 // const VkDescriptorBufferInfo  *pBufferInfo
            nullptr                                       // const VkBufferView            *pTexelBufferView
        }
    };

    vkUpdateDescriptorSets( GetDevice(), static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr );
    return true;
}

bool Tutorial07::CreateRenderPass()
{
    VkAttachmentDescription attachment_descriptions[] =
    {
        {
            0,                                          // VkAttachmentDescriptionFlags   flags
            GetSwapChain().Format,                      // VkFormat                       format
            VK_SAMPLE_COUNT_1_BIT,                      // VkSampleCountFlagBits          samples
            VK_ATTACHMENT_LOAD_OP_CLEAR,                // VkAttachmentLoadOp             loadOp
            VK_ATTACHMENT_STORE_OP_STORE,               // VkAttachmentStoreOp            storeOp
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,            // VkAttachmentLoadOp             stencilLoadOp
            VK_ATTACHMENT_STORE_OP_DONT_CARE,           // VkAttachmentStoreOp            stencilStoreOp
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout                  initialLayout;
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL    // VkImageLayout                  finalLayout
        }
    };

    VkAttachmentReference color_attachment_references[] =
    {
        {
            0,                                          // uint32_t                       attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL    // VkImageLayout                  layout
        }
    };

    VkSubpassDescription subpass_descriptions[] =
    {
        {
            0,                                          // VkSubpassDescriptionFlags      flags
            VK_PIPELINE_BIND_POINT_GRAPHICS,            // VkPipelineBindPoint            pipelineBindPoint
            0,                                          // uint32_t                       inputAttachmentCount
            nullptr,                                    // const VkAttachmentReference   *pInputAttachments
            1,                                          // uint32_t                       colorAttachmentCount
            color_attachment_references,                // const VkAttachmentReference   *pColorAttachments
            nullptr,                                    // const VkAttachmentReference   *pResolveAttachments
            nullptr,                                    // const VkAttachmentReference   *pDepthStencilAttachment
            0,                                          // uint32_t                       preserveAttachmentCount
            nullptr                                     // const uint32_t*                pPreserveAttachments
        }
    };

    VkRenderPassCreateInfo render_pass_create_info =
    {
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,    // VkStructureType                sType
        nullptr,                                      // const void                    *pNext
        0,                                            // VkRenderPassCreateFlags        flags
        1,                                            // uint32_t                       attachmentCount
        attachment_descriptions,                      // const VkAttachmentDescription *pAttachments
        1,                                            // uint32_t                       subpassCount
        subpass_descriptions,                         // const VkSubpassDescription    *pSubpasses
        0,                                            // uint32_t                       dependencyCount
        nullptr                                       // const VkSubpassDependency     *pDependencies
    };

    if( vkCreateRenderPass( GetDevice(), &render_pass_create_info, nullptr, &Vulkan.RenderPass ) != VK_SUCCESS )
    {
        std::cout << "Could not create render pass!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo layout_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,  // VkStructureType                sType
        nullptr,                                        // const void                    *pNext
        0,                                              // VkPipelineLayoutCreateFlags    flags
        1,                                              // uint32_t                       setLayoutCount
        &Vulkan.DescriptorSet.Layout,                   // const VkDescriptorSetLayout   *pSetLayouts
        0,                                              // uint32_t                       pushConstantRangeCount
        nullptr                                         // const VkPushConstantRange     *pPushConstantRanges
    };

    if( vkCreatePipelineLayout( GetDevice(), &layout_create_info, nullptr, &Vulkan.PipelineLayout ) != VK_SUCCESS )
    {
        std::cout << "Could not create pipeline layout!" << std::endl;
        return false;
    }
    return true;
}

bool Tutorial07::CreatePipeline()
{
	// DRE 2024 - Changed file folder locations
    ///Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> vertex_shader_module = CreateShaderModule( "Data/Tutorials/07/shader.vert.spv" );
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> vertex_shader_module = CreateShaderModule( "Data/shader.vert.spv" );
    ///Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> fragment_shader_module = CreateShaderModule( "Data/Tutorials/07/shader.frag.spv" );
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> fragment_shader_module = CreateShaderModule( "Data/shader.frag.spv" );

    if( !vertex_shader_module || !fragment_shader_module )
    {
        return false;
    }

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos =
    {
        // Vertex shader
        {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // VkStructureType                                sType
            nullptr,                                                    // const void                                    *pNext
            0,                                                          // VkPipelineShaderStageCreateFlags               flags
            VK_SHADER_STAGE_VERTEX_BIT,                                 // VkShaderStageFlagBits                          stage
            vertex_shader_module.Get(),                                 // VkShaderModule                                 module
            "main",                                                     // const char                                    *pName
            nullptr                                                     // const VkSpecializationInfo                    *pSpecializationInfo
        },
        // Fragment shader
        {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // VkStructureType                                sType
            nullptr,                                                    // const void                                    *pNext
            0,                                                          // VkPipelineShaderStageCreateFlags               flags
            VK_SHADER_STAGE_FRAGMENT_BIT,                               // VkShaderStageFlagBits                          stage
            fragment_shader_module.Get(),                               // VkShaderModule                                 module
            "main",                                                     // const char                                    *pName
            nullptr                                                     // const VkSpecializationInfo                    *pSpecializationInfo
        }
    };

    VkVertexInputBindingDescription vertex_binding_description =
    {
        0,                                                            // uint32_t                                       binding
        sizeof(VertexData),                                           // uint32_t                                       stride
        VK_VERTEX_INPUT_RATE_VERTEX                                   // VkVertexInputRate                              inputRate
    };

    VkVertexInputAttributeDescription vertex_attribute_descriptions[] =
    {
        {
            0,                                                          // uint32_t                                       location
            vertex_binding_description.binding,                         // uint32_t                                       binding
            VK_FORMAT_R32G32B32A32_SFLOAT,                              // VkFormat                                       format
            0                                                           // uint32_t                                       offset
        },
        {
            1,                                                          // uint32_t                                       location
            vertex_binding_description.binding,                         // uint32_t                                       binding
            VK_FORMAT_R32G32_SFLOAT,                                    // VkFormat                                       format
            4 * sizeof(float)                                           // uint32_t                                       offset
        }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,    // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineVertexInputStateCreateFlags          flags;
        1,                                                            // uint32_t                                       vertexBindingDescriptionCount
        &vertex_binding_description,                                  // const VkVertexInputBindingDescription         *pVertexBindingDescriptions
        2,                                                            // uint32_t                                       vertexAttributeDescriptionCount
        vertex_attribute_descriptions                                 // const VkVertexInputAttributeDescription       *pVertexAttributeDescriptions
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,  // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineInputAssemblyStateCreateFlags        flags
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,                         // VkPrimitiveTopology                            topology
        VK_FALSE                                                      // VkBool32                                       primitiveRestartEnable
    };

    VkPipelineViewportStateCreateInfo viewport_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,        // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineViewportStateCreateFlags             flags
        1,                                                            // uint32_t                                       viewportCount
        nullptr,                                                      // const VkViewport                              *pViewports
        1,                                                            // uint32_t                                       scissorCount
        nullptr                                                       // const VkRect2D                                *pScissors
    };

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,   // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineRasterizationStateCreateFlags        flags
        VK_FALSE,                                                     // VkBool32                                       depthClampEnable
        VK_FALSE,                                                     // VkBool32                                       rasterizerDiscardEnable
        VK_POLYGON_MODE_FILL,                                         // VkPolygonMode                                  polygonMode
        VK_CULL_MODE_BACK_BIT,                                        // VkCullModeFlags                                cullMode
        VK_FRONT_FACE_COUNTER_CLOCKWISE,                              // VkFrontFace                                    frontFace
        VK_FALSE,                                                     // VkBool32                                       depthBiasEnable
        0.0f,                                                         // float                                          depthBiasConstantFactor
        0.0f,                                                         // float                                          depthBiasClamp
        0.0f,                                                         // float                                          depthBiasSlopeFactor
        1.0f                                                          // float                                          lineWidth
    };

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,     // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineMultisampleStateCreateFlags          flags
        VK_SAMPLE_COUNT_1_BIT,                                        // VkSampleCountFlagBits                          rasterizationSamples
        VK_FALSE,                                                     // VkBool32                                       sampleShadingEnable
        1.0f,                                                         // float                                          minSampleShading
        nullptr,                                                      // const VkSampleMask                            *pSampleMask
        VK_FALSE,                                                     // VkBool32                                       alphaToCoverageEnable
        VK_FALSE                                                      // VkBool32                                       alphaToOneEnable
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment_state =
    {
        VK_FALSE,                                                     // VkBool32                                       blendEnable
        VK_BLEND_FACTOR_ONE,                                          // VkBlendFactor                                  srcColorBlendFactor
        VK_BLEND_FACTOR_ZERO,                                         // VkBlendFactor                                  dstColorBlendFactor
        VK_BLEND_OP_ADD,                                              // VkBlendOp                                      colorBlendOp
        VK_BLEND_FACTOR_ONE,                                          // VkBlendFactor                                  srcAlphaBlendFactor
        VK_BLEND_FACTOR_ZERO,                                         // VkBlendFactor                                  dstAlphaBlendFactor
        VK_BLEND_OP_ADD,                                              // VkBlendOp                                      alphaBlendOp
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |         // VkColorComponentFlags                          colorWriteMask
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,     // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineColorBlendStateCreateFlags           flags
        VK_FALSE,                                                     // VkBool32                                       logicOpEnable
        VK_LOGIC_OP_COPY,                                             // VkLogicOp                                      logicOp
        1,                                                            // uint32_t                                       attachmentCount
        &color_blend_attachment_state,                                // const VkPipelineColorBlendAttachmentState     *pAttachments
        { 0.0f, 0.0f, 0.0f, 0.0f }                                    // float                                          blendConstants[4]
    };

    VkDynamicState dynamic_states[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info =
    {
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,         // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineDynamicStateCreateFlags              flags
        2,                                                            // uint32_t                                       dynamicStateCount
        dynamic_states                                                // const VkDynamicState                          *pDynamicStates
    };

    VkGraphicsPipelineCreateInfo pipeline_create_info =
    {
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,              // VkStructureType                                sType
        nullptr,                                                      // const void                                    *pNext
        0,                                                            // VkPipelineCreateFlags                          flags
        static_cast<uint32_t>(shader_stage_create_infos.size()),      // uint32_t                                       stageCount
        shader_stage_create_infos.data(),                             // const VkPipelineShaderStageCreateInfo         *pStages
        &vertex_input_state_create_info,                              // const VkPipelineVertexInputStateCreateInfo    *pVertexInputState;
        &input_assembly_state_create_info,                            // const VkPipelineInputAssemblyStateCreateInfo  *pInputAssemblyState
        nullptr,                                                      // const VkPipelineTessellationStateCreateInfo   *pTessellationState
        &viewport_state_create_info,                                  // const VkPipelineViewportStateCreateInfo       *pViewportState
        &rasterization_state_create_info,                             // const VkPipelineRasterizationStateCreateInfo  *pRasterizationState
        &multisample_state_create_info,                               // const VkPipelineMultisampleStateCreateInfo    *pMultisampleState
        nullptr,                                                      // const VkPipelineDepthStencilStateCreateInfo   *pDepthStencilState
        &color_blend_state_create_info,                               // const VkPipelineColorBlendStateCreateInfo     *pColorBlendState
        &dynamic_state_create_info,                                   // const VkPipelineDynamicStateCreateInfo        *pDynamicState
        Vulkan.PipelineLayout,                                        // VkPipelineLayout                               layout
        Vulkan.RenderPass,                                            // VkRenderPass                                   renderPass
        0,                                                            // uint32_t                                       subpass
        VK_NULL_HANDLE,                                               // VkPipeline                                     basePipelineHandle
        -1                                                            // int32_t                                        basePipelineIndex
    };

    if( vkCreateGraphicsPipelines( GetDevice(), VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &Vulkan.GraphicsPipeline ) != VK_SUCCESS )
    {
        std::cout << "Could not create graphics pipeline!" << std::endl;
        return false;
    }
    return true;
}

Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> Tutorial07::CreateShaderModule( const char* filename )
{
    const std::vector<char> code = Tools::GetBinaryFileContents( filename );
    if( code.size() == 0 )
    {
        return Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>();
    }

    VkShaderModuleCreateInfo shader_module_create_info =
    {
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,    // VkStructureType                sType
        nullptr,                                        // const void                    *pNext
        0,                                              // VkShaderModuleCreateFlags      flags
        code.size(),                                    // size_t                         codeSize
        reinterpret_cast<const uint32_t*>(code.data())  // const uint32_t                *pCode
    };

    VkShaderModule shader_module;
    if( vkCreateShaderModule( GetDevice(), &shader_module_create_info, nullptr, &shader_module ) != VK_SUCCESS )
    {
        std::cout << "Could not create shader module from a \"" << filename << "\" file!" << std::endl;
        return Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>();
    }

    return Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>( shader_module, vkDestroyShaderModule, GetDevice() );
}

bool Tutorial07::CreateVertexBuffer()
{
    const std::vector<float>& vertex_data = GetVertexData();

    Vulkan.VertexBuffer.Size = static_cast<uint32_t>(vertex_data.size() * sizeof(vertex_data[0]));
    if( !CreateBuffer( VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Vulkan.VertexBuffer ) )
    {
        std::cout << "Could not create vertex buffer!" << std::endl;
        return false;
    }

    if( !CopyVertexData() )
    {
        return false;
    }

    return true;
}

const std::vector<float>& Tutorial07::GetVertexData() const
{
    static const std::vector<float> vertex_data =
    {
        -170.0f, -170.0f, 0.0f, 1.0f,
        -0.1f, -0.1f,
        //
        -170.0f, 170.0f, 0.0f, 1.0f,
        -0.1f, 1.1f,
        //
        170.0f, -170.0f, 0.0f, 1.0f,
        1.1f, -0.1f,
        //
        170.0f, 170.0f, 0.0f, 1.0f,
        1.1f, 1.1f,
    };

    return vertex_data;
}

bool Tutorial07::CopyVertexData()
{
    // Prepare data in staging buffer
    const std::vector<float>& vertex_data = GetVertexData();

    void *staging_buffer_memory_pointer;
    if( vkMapMemory( GetDevice(), Vulkan.StagingBuffer.Memory, 0, Vulkan.VertexBuffer.Size, 0, &staging_buffer_memory_pointer) != VK_SUCCESS )
    {
        std::cout << "Could not map memory and upload data to a staging buffer!" << std::endl;
        return false;
    }

    memcpy( staging_buffer_memory_pointer, vertex_data.data(), Vulkan.VertexBuffer.Size );

    VkMappedMemoryRange flush_range =
    {
        VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,              // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        Vulkan.StagingBuffer.Memory,                        // VkDeviceMemory                         memory
        0,                                                  // VkDeviceSize                           offset
        Vulkan.VertexBuffer.Size                            // VkDeviceSize                           size
    };
    vkFlushMappedMemoryRanges( GetDevice(), 1, &flush_range );

    vkUnmapMemory( GetDevice(), Vulkan.StagingBuffer.Memory );

    // Prepare command buffer to copy data from staging buffer to a vertex buffer
    VkCommandBufferBeginInfo command_buffer_begin_info =
    {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,        // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,        // VkCommandBufferUsageFlags              flags
        nullptr                                             // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    VkCommandBuffer command_buffer = Vulkan.RenderingResources[0].CommandBuffer;

    vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info);

    VkBufferCopy buffer_copy_info =
    {
        0,                                                  // VkDeviceSize                           srcOffset
        0,                                                  // VkDeviceSize                           dstOffset
        Vulkan.VertexBuffer.Size                            // VkDeviceSize                           size
    };
    vkCmdCopyBuffer(command_buffer, Vulkan.StagingBuffer.Handle, Vulkan.VertexBuffer.Handle, 1, &buffer_copy_info);

    VkBufferMemoryBarrier buffer_memory_barrier =
    {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,            // VkStructureType                        sType;
        nullptr,                                            // const void                            *pNext
        VK_ACCESS_MEMORY_WRITE_BIT,                         // VkAccessFlags                          srcAccessMask
        VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,                // VkAccessFlags                          dstAccessMask
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED,                            // uint32_t                               dstQueueFamilyIndex
        Vulkan.VertexBuffer.Handle,                         // VkBuffer                               buffer
        0,                                                  // VkDeviceSize                           offset
        VK_WHOLE_SIZE                                       // VkDeviceSize                           size
    };
    vkCmdPipelineBarrier( command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, nullptr, 1, &buffer_memory_barrier, 0, nullptr );

    vkEndCommandBuffer( command_buffer );

    // Submit command buffer and copy data from staging buffer to a vertex buffer
    VkSubmitInfo submit_info =
    {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,                      // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        0,                                                  // uint32_t                               waitSemaphoreCount
        nullptr,                                            // const VkSemaphore                     *pWaitSemaphores
        nullptr,                                            // const VkPipelineStageFlags            *pWaitDstStageMask;
        1,                                                  // uint32_t                               commandBufferCount
        &command_buffer,                                    // const VkCommandBuffer                 *pCommandBuffers
        0,                                                  // uint32_t                               signalSemaphoreCount
        nullptr                                             // const VkSemaphore                     *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_info, VK_NULL_HANDLE ) != VK_SUCCESS )
    {
        return false;
    }

    vkDeviceWaitIdle( GetDevice() );

    return true;
}

bool Tutorial07::PrepareFrame( VkCommandBuffer command_buffer, const ImageParameters &image_parameters, VkFramebuffer &framebuffer )
{
    if( !CreateFramebuffer( framebuffer, image_parameters.View ) )
    {
        return false;
    }

    VkCommandBufferBeginInfo command_buffer_begin_info =
    {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,        // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,        // VkCommandBufferUsageFlags              flags
        nullptr                                             // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info );

    VkImageSubresourceRange image_subresource_range =
    {
        VK_IMAGE_ASPECT_COLOR_BIT,                          // VkImageAspectFlags                     aspectMask
        0,                                                  // uint32_t                               baseMipLevel
        1,                                                  // uint32_t                               levelCount
        0,                                                  // uint32_t                               baseArrayLayer
        1                                                   // uint32_t                               layerCount
    };

    uint32_t present_queue_family_index = (GetPresentQueue().Handle != GetGraphicsQueue().Handle) ? GetPresentQueue().FamilyIndex : VK_QUEUE_FAMILY_IGNORED;
    uint32_t graphics_queue_family_index = (GetPresentQueue().Handle != GetGraphicsQueue().Handle) ? GetGraphicsQueue().FamilyIndex : VK_QUEUE_FAMILY_IGNORED;
    VkImageMemoryBarrier barrier_from_present_to_draw =
    {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,             // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_ACCESS_MEMORY_READ_BIT,                          // VkAccessFlags                          srcAccessMask
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
        VK_IMAGE_LAYOUT_UNDEFINED,                          // VkImageLayout                          oldLayout
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,           // VkImageLayout                          newLayout
        present_queue_family_index,                         // uint32_t                               srcQueueFamilyIndex
        graphics_queue_family_index,                        // uint32_t                               dstQueueFamilyIndex
        image_parameters.Handle,                            // VkImage                                image
        image_subresource_range                             // VkImageSubresourceRange                subresourceRange
    };
    vkCmdPipelineBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_draw );

    VkClearValue clear_value =
    {
        { 1.0f, 0.8f, 0.4f, 0.0f },                         // VkClearColorValue                      color
    };

    VkRenderPassBeginInfo render_pass_begin_info =
    {
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,           // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        Vulkan.RenderPass,                                  // VkRenderPass                           renderPass
        framebuffer,                                        // VkFramebuffer                          framebuffer
        {
            // VkRect2D                               renderArea
            {
                // VkOffset2D                             offset
                0,                                                // int32_t                                x
                0                                                 // int32_t                                y
            },
            GetSwapChain().Extent,                            // VkExtent2D                             extent;
        },
        1,                                                  // uint32_t                               clearValueCount
        &clear_value                                        // const VkClearValue                    *pClearValues
    };

    vkCmdBeginRenderPass( command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Vulkan.GraphicsPipeline );

    VkViewport viewport =
    {
        0.0f,                                               // float                                  x
        0.0f,                                               // float                                  y
        static_cast<float>(GetSwapChain().Extent.width),    // float                                  width
        static_cast<float>(GetSwapChain().Extent.height),   // float                                  height
        0.0f,                                               // float                                  minDepth
        1.0f                                                // float                                  maxDepth
    };

    VkRect2D scissor =
    {
        {
            // VkOffset2D                             offset
            0,                                                  // int32_t                                x
            0                                                   // int32_t                                y
        },
        {
            // VkExtent2D                             extent
            GetSwapChain().Extent.width,                        // uint32_t                               width
            GetSwapChain().Extent.height                        // uint32_t                               height
        }
    };

    vkCmdSetViewport( command_buffer, 0, 1, &viewport );
    vkCmdSetScissor( command_buffer, 0, 1, &scissor );

    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers( command_buffer, 0, 1, &Vulkan.VertexBuffer.Handle, &offset );

    vkCmdBindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Vulkan.PipelineLayout, 0, 1, &Vulkan.DescriptorSet.Handle, 0, nullptr );

    vkCmdDraw( command_buffer, 4, 1, 0, 0 );

    vkCmdEndRenderPass( command_buffer );

    VkImageMemoryBarrier barrier_from_draw_to_present =
    {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,             // VkStructureType                        sType
        nullptr,                                            // const void                            *pNext
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
        VK_ACCESS_MEMORY_READ_BIT,                          // VkAccessFlags                          dstAccessMask
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,           // VkImageLayout                          oldLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,                    // VkImageLayout                          newLayout
        graphics_queue_family_index,                        // uint32_t                               srcQueueFamilyIndex
        present_queue_family_index,                         // uint32_t                               dstQueueFamilyIndex
        image_parameters.Handle,                            // VkImage                                image
        image_subresource_range                             // VkImageSubresourceRange                subresourceRange
    };
    vkCmdPipelineBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_draw_to_present );

    if( vkEndCommandBuffer( command_buffer ) != VK_SUCCESS )
    {
        std::cout << "Could not record command buffer!" << std::endl;
        return false;
    }
    return true;
}

bool Tutorial07::CreateFramebuffer( VkFramebuffer &framebuffer, VkImageView image_view )
{
    if( framebuffer != VK_NULL_HANDLE )
    {
        vkDestroyFramebuffer( GetDevice(), framebuffer, nullptr );
        framebuffer = VK_NULL_HANDLE;
    }

    VkFramebufferCreateInfo framebuffer_create_info =
    {
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,      // VkStructureType                sType
        nullptr,                                        // const void                    *pNext
        0,                                              // VkFramebufferCreateFlags       flags
        Vulkan.RenderPass,                              // VkRenderPass                   renderPass
        1,                                              // uint32_t                       attachmentCount
        &image_view,                                    // const VkImageView             *pAttachments
        GetSwapChain().Extent.width,                    // uint32_t                       width
        GetSwapChain().Extent.height,                   // uint32_t                       height
        1                                               // uint32_t                       layers
    };

    if( vkCreateFramebuffer( GetDevice(), &framebuffer_create_info, nullptr, &framebuffer ) != VK_SUCCESS )
    {
        std::cout << "Could not create a framebuffer!" << std::endl;
        return false;
    }

    return true;
}

bool Tutorial07::ChildOnWindowSizeChanged()
{
    if( (GetDevice() != VK_NULL_HANDLE) && (Vulkan.StagingBuffer.Handle != VK_NULL_HANDLE) )
    {
        vkDeviceWaitIdle( GetDevice() );
        return CopyUniformBufferData();
    }
    return true;
}

bool Tutorial07::Draw()
{
    static size_t           resource_index = 0;
    RenderingResourcesData &current_rendering_resource = Vulkan.RenderingResources[resource_index];
    VkSwapchainKHR          swap_chain = GetSwapChain().Handle;
    uint32_t                image_index;

    resource_index = (resource_index + 1) % VulkanTutorial07Parameters::ResourcesCount;

    if( vkWaitForFences( GetDevice(), 1, &current_rendering_resource.Fence, VK_FALSE, 1000000000 ) != VK_SUCCESS )
    {
        std::cout << "Waiting for fence takes too long!" << std::endl;
        return false;
    }
    vkResetFences( GetDevice(), 1, &current_rendering_resource.Fence );

    VkResult result = vkAcquireNextImageKHR( GetDevice(), swap_chain, UINT64_MAX, current_rendering_resource.ImageAvailableSemaphore, VK_NULL_HANDLE, &image_index );
    switch( result )
    {
		case VK_SUCCESS:
		case VK_SUBOPTIMAL_KHR:
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			return OnWindowSizeChanged();
		default:
			std::cout << "Problem occurred during swap chain image acquisition!" << std::endl;
			return false;
    }

    if( !PrepareFrame( current_rendering_resource.CommandBuffer, GetSwapChain().Images[image_index], current_rendering_resource.Framebuffer ) )
    {
        return false;
    }

    VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info =
    {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,                          // VkStructureType              sType
        nullptr,                                                // const void                  *pNext
        1,                                                      // uint32_t                     waitSemaphoreCount
        &current_rendering_resource.ImageAvailableSemaphore,    // const VkSemaphore           *pWaitSemaphores
        &wait_dst_stage_mask,                                   // const VkPipelineStageFlags  *pWaitDstStageMask;
        1,                                                      // uint32_t                     commandBufferCount
        &current_rendering_resource.CommandBuffer,              // const VkCommandBuffer       *pCommandBuffers
        1,                                                      // uint32_t                     signalSemaphoreCount
        &current_rendering_resource.FinishedRenderingSemaphore  // const VkSemaphore           *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_info, current_rendering_resource.Fence ) != VK_SUCCESS )
    {
        return false;
    }

    VkPresentInfoKHR present_info =
    {
        VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,                     // VkStructureType              sType
        nullptr,                                                // const void                  *pNext
        1,                                                      // uint32_t                     waitSemaphoreCount
        &current_rendering_resource.FinishedRenderingSemaphore, // const VkSemaphore           *pWaitSemaphores
        1,                                                      // uint32_t                     swapchainCount
        &swap_chain,                                            // const VkSwapchainKHR        *pSwapchains
        &image_index,                                           // const uint32_t              *pImageIndices
        nullptr                                                 // VkResult                    *pResults
    };
    result = vkQueuePresentKHR( GetPresentQueue().Handle, &present_info );

    switch( result )
    {
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
		case VK_SUBOPTIMAL_KHR:
			return OnWindowSizeChanged();
		default:
			std::cout << "Problem occurred during image presentation!" << std::endl;
			return false;
    }

    return true;
}

void Tutorial07::ChildClear()
{
}

Tutorial07::~Tutorial07()
{
    if( GetDevice() != VK_NULL_HANDLE )
    {
        vkDeviceWaitIdle( GetDevice() );

        for( size_t i = 0; i < Vulkan.RenderingResources.size(); ++i )
        {
            if( Vulkan.RenderingResources[i].Framebuffer != VK_NULL_HANDLE )
            {
                vkDestroyFramebuffer( GetDevice(), Vulkan.RenderingResources[i].Framebuffer, nullptr );
            }
            if( Vulkan.RenderingResources[i].CommandBuffer != VK_NULL_HANDLE )
            {
                vkFreeCommandBuffers( GetDevice(), Vulkan.CommandPool, 1, &Vulkan.RenderingResources[i].CommandBuffer );
            }
            if( Vulkan.RenderingResources[i].ImageAvailableSemaphore != VK_NULL_HANDLE )
            {
                vkDestroySemaphore( GetDevice(), Vulkan.RenderingResources[i].ImageAvailableSemaphore, nullptr );
            }
            if( Vulkan.RenderingResources[i].FinishedRenderingSemaphore != VK_NULL_HANDLE )
            {
                vkDestroySemaphore( GetDevice(), Vulkan.RenderingResources[i].FinishedRenderingSemaphore, nullptr );
            }
            if( Vulkan.RenderingResources[i].Fence != VK_NULL_HANDLE )
            {
                vkDestroyFence( GetDevice(), Vulkan.RenderingResources[i].Fence, nullptr );
            }
        }

        if( Vulkan.CommandPool != VK_NULL_HANDLE )
        {
            vkDestroyCommandPool( GetDevice(), Vulkan.CommandPool, nullptr );
            Vulkan.CommandPool = VK_NULL_HANDLE;
        }

        DestroyBuffer( Vulkan.VertexBuffer );

        DestroyBuffer( Vulkan.StagingBuffer );

        if( Vulkan.GraphicsPipeline != VK_NULL_HANDLE )
        {
            vkDestroyPipeline( GetDevice(), Vulkan.GraphicsPipeline, nullptr );
            Vulkan.GraphicsPipeline = VK_NULL_HANDLE;
        }

        if( Vulkan.PipelineLayout != VK_NULL_HANDLE )
        {
            vkDestroyPipelineLayout( GetDevice(), Vulkan.PipelineLayout, nullptr );
            Vulkan.PipelineLayout = VK_NULL_HANDLE;
        }

        if( Vulkan.RenderPass != VK_NULL_HANDLE )
        {
            vkDestroyRenderPass( GetDevice(), Vulkan.RenderPass, nullptr );
            Vulkan.RenderPass = VK_NULL_HANDLE;
        }

        if( Vulkan.DescriptorSet.Pool != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorPool( GetDevice(), Vulkan.DescriptorSet.Pool, nullptr );
            Vulkan.DescriptorSet.Pool = VK_NULL_HANDLE;
        }

        if( Vulkan.DescriptorSet.Layout != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorSetLayout( GetDevice(), Vulkan.DescriptorSet.Layout, nullptr );
            Vulkan.DescriptorSet.Layout = VK_NULL_HANDLE;
        }

        DestroyBuffer( Vulkan.UniformBuffer );

        if( Vulkan.Image.Sampler != VK_NULL_HANDLE )
        {
            vkDestroySampler( GetDevice(), Vulkan.Image.Sampler, nullptr );
            Vulkan.Image.Sampler = VK_NULL_HANDLE;
        }

        if( Vulkan.Image.View != VK_NULL_HANDLE )
        {
            vkDestroyImageView( GetDevice(), Vulkan.Image.View, nullptr );
            Vulkan.Image.View = VK_NULL_HANDLE;
        }

        if( Vulkan.Image.Handle != VK_NULL_HANDLE )
        {
            vkDestroyImage( GetDevice(), Vulkan.Image.Handle, nullptr );
            Vulkan.Image.Handle = VK_NULL_HANDLE;
        }

        if( Vulkan.Image.Memory != VK_NULL_HANDLE )
        {
            vkFreeMemory( GetDevice(), Vulkan.Image.Memory, nullptr );
            Vulkan.Image.Memory = VK_NULL_HANDLE;
        }
    }
}

void Tutorial07::DestroyBuffer( BufferParameters& buffer )
{
    if( buffer.Handle != VK_NULL_HANDLE )
    {
        vkDestroyBuffer( GetDevice(), buffer.Handle, nullptr );
        buffer.Handle = VK_NULL_HANDLE;
    }

    if( buffer.Memory != VK_NULL_HANDLE )
    {
        vkFreeMemory( GetDevice(), buffer.Memory, nullptr );
        buffer.Memory = VK_NULL_HANDLE;
    }
}

} // namespace ApiWithoutSecrets
