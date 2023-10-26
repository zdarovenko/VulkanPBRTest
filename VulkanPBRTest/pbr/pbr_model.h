#pragma once

#include "pbr_device.h"
#include "pbr_buffer.h"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

// std
#include <vector>
#include <memory>

namespace pbr
{
	class PbrModel
	{
	public:

		struct Vertex
		{
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		bool hasIndexBuffer = false;

		PbrModel(PbrDevice&, const Builder&);
		~PbrModel();

		PbrModel(const PbrModel&) = delete;
		PbrModel& operator=(const PbrModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

		static std::unique_ptr<PbrModel> createModelFromFile(PbrDevice& device, const std::string& filepath);

	private:
		PbrDevice& pbrDevice;
		std::unique_ptr<PbrBuffer> vertexBuffer;
		uint32_t vertexCount;

		std::unique_ptr<PbrBuffer> indexBuffer;
		uint32_t indexCount;

		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);
	};
}