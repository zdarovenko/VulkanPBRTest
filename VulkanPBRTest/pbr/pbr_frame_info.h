#pragma once

#include "pbr_camera.h"
#include "pbr_game_object.h"

#include <vulkan/vulkan.h>

namespace pbr
{

#define	MAX_LIGHTS 10

	struct PointLight
	{
		glm::vec4 position{};
		glm::vec4 color{};
	};

	struct GlobalUbo
	{
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 inverseView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		PbrCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		PbrGameObject::Map& gameObjects;
	};
}
