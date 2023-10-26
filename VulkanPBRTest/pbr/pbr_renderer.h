#pragma once

#include "pbr_window.h"
#include "pbr_device.h"
#include "pbr_swap_chain.h"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace pbr
{
	class PbrRenderer
	{
	public:

		PbrRenderer(PbrWindow&, PbrDevice&);
		~PbrRenderer();

		PbrRenderer(const PbrRenderer&) = delete;
		PbrRenderer& operator=(const PbrRenderer&) = delete;

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		VkRenderPass getSwapChainRenderPass() const { return pbrSwapChain->getRenderPass(); }

		bool isFrameInProgress() { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame is not in progress!");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame is not in progress!");

			return currentFrameIndex;
		}

		float getAspectRatio() { return pbrSwapChain->extentAspectRatio(); }

	private:
		PbrWindow& pbrWindow;
		PbrDevice& pbrDevice;
		std::unique_ptr<PbrSwapChain> pbrSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
	};
}