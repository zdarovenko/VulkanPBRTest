#include "first_app.h"
#include "keyboard_movement_controller.hpp"
#include "systems/simple_render_system.h"
#include "systems/point_light_system.h"
#include "pbr_camera.h"
#include "pbr_buffer.h"
#include "pbr_frame_info.h"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

// std
#include <stdexcept>
#include <array>
#include <cassert>
#include <chrono>

namespace pbr
{
	FirstApp::FirstApp()
	{
		globalPool = PbrDescriptorPool::Builder(pbrDevice)
			.setMaxSets(PbrSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, PbrSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, PbrSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<PbrBuffer>> uboBuffers(PbrSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < PbrSwapChain::MAX_FRAMES_IN_FLIGHT; ++i)
		{
			uboBuffers[i] = std::make_unique<PbrBuffer>(
				pbrDevice,
				sizeof(GlobalUbo),
				PbrSwapChain::MAX_FRAMES_IN_FLIGHT,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);

			uboBuffers[i]->map();
		}

		auto globalSetLayout = PbrDescriptorSetLayout::Builder(pbrDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(PbrSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < globalDescriptorSets.size(); ++i)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();

			PbrDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{ pbrDevice, pbrRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		PointLightSystem pointLightSystem{ pbrDevice, pbrRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
        PbrCamera camera{};

        camera.setViewDirection(glm::vec3{ 0.f }, glm::vec3{ .5f, 0.f, 1.f });

        auto viewerObject = PbrGameObject::createGameObject();
		viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!pbrWindow.shouldClose())
		{
            glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(pbrWindow.getGLFWwindow(), frameTime, viewerObject);

            float aspect = pbrRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			if (auto commandBuffer = pbrRenderer.beginFrame())
			{
				int frameIndex = pbrRenderer.getFrameIndex();
				
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects
				};

				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				ubo.inverseView = camera.getInverseView();
				pointLightSystem.update(frameInfo, ubo);
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush(frameIndex);

				pbrRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				pbrRenderer.endSwapChainRenderPass(commandBuffer);
				pbrRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(pbrDevice.device());
	}

    void FirstApp::loadGameObjects()
    {
		std::shared_ptr<PbrModel> flatVaseModel = PbrModel::createModelFromFile(pbrDevice, "pbr/models/flat_vase.obj");

        auto flatVase = PbrGameObject::createGameObject();
		flatVase.model = flatVaseModel;
		flatVase.transform.scale = { 3.f, 1.5f, 3.f };
		flatVase.transform.translation = { .5f, .5f, 0.f };

		gameObjects.emplace(flatVase.getId(), std::move(flatVase));

		std::shared_ptr<PbrModel> smoothVaseModel = PbrModel::createModelFromFile(pbrDevice, "pbr/models/smooth_vase.obj");

		auto smoothVase = PbrGameObject::createGameObject();
		smoothVase.model = smoothVaseModel;
		smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
		smoothVase.transform.translation = { -.5f, .5f, 0.f };

		gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

		std::shared_ptr<PbrModel> floorModel = PbrModel::createModelFromFile(pbrDevice, "pbr/models/quad.obj");

		auto floor = PbrGameObject::createGameObject();
		floor.model = floorModel;
		floor.transform.scale = { 3.f, 1.5f, 3.f };
		floor.transform.translation = { 0.f, .5f, 0.f };

		gameObjects.emplace(floor.getId(), std::move(floor));

		std::vector<glm::vec3> lightColors
		{
			 {1.f, .1f, .1f},
			 {.1f, .1f, 1.f},
			 {.1f, 1.f, .1f},
			 {1.f, 1.f, .1f},
			 {.1f, 1.f, 1.f},
			 {1.f, 1.f, 1.f}
		};

		for (int i = 0; i < lightColors.size(); ++i)
		{
			auto pointLight = PbrGameObject::makePointLight(0.2f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), { 0.f, -1.f, 0.f });
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4{ -1.f, -1.f, -1.f, 1.f });
			gameObjects.emplace(pointLight.getId(), std::move(pointLight));
		}
	}
}