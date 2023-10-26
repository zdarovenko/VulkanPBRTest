#pragma once

#include "../pbr_pipeline.h"
#include "../pbr_device.h"
#include "../pbr_game_object.h"
#include "../pbr_camera.h"
#include "../pbr_frame_info.h"

// std
#include <memory>
#include <vector>

namespace pbr
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(PbrDevice&, VkRenderPass, VkDescriptorSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo);

	private:
		PbrDevice& pbrDevice;
		std::unique_ptr<PbrPipeline> pbrPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<PbrGameObject> gameObjects;

		void createPipelineLayout(VkDescriptorSetLayout);
		void createPipeline(VkRenderPass);
	};
}