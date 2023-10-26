#pragma once

// pbr
#include "pbr_device.h"

//std
#include <string>
#include <vector>

namespace pbr
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineViewportStateCreateInfo viewportInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class PbrPipeline
	{
	public:
		PbrPipeline(
			PbrDevice&, 
			const std::string&,
			const std::string&,
			const PipelineConfigInfo&
		);
		~PbrPipeline();
		PbrPipeline(const PbrPipeline&) = delete;
		void operator=(const PbrPipeline&) = delete;

		void bind(VkCommandBuffer);

		static void defaultPipelineConfigInfo(PipelineConfigInfo&);
		static void enableColorBlending(PipelineConfigInfo&);


	private:
		PbrDevice& pbrDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		static std::vector<char> readFile(const std::string&);

		void createGraphicsPipeline(const std::string&, const std::string&, const PipelineConfigInfo&);

		void createShaderModule(const std::vector<char>&, VkShaderModule*);
	};
}