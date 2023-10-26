#pragma once

#include "pbr_window.h"
#include "pbr_pipeline.h"
#include "pbr_device.h"
#include "pbr_renderer.h"
#include "pbr_game_object.h"
#include "pbr_descriptors.h"

// std
#include <memory>
#include <vector>

namespace pbr
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		PbrWindow pbrWindow{ WIDTH, HEIGHT, "Vulkan PBR test!" };
		PbrDevice pbrDevice{ pbrWindow };
		PbrRenderer pbrRenderer{ pbrWindow, pbrDevice };

		std::unique_ptr<PbrDescriptorPool> globalPool{};
		PbrGameObject::Map gameObjects;

		void loadGameObjects();
	};
}