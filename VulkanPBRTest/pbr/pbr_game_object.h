#pragma once

// glm
#include <glm/gtc/matrix_transform.hpp>

// pbr
#include "pbr_model.h"

// std
#include <memory>
#include <unordered_map>

namespace pbr
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation;

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	struct PointLightComponent
	{
		float lightIntensity = 1.0f;
	};

	class PbrGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, PbrGameObject>;

		std::shared_ptr<PbrModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

		std::unique_ptr<PointLightComponent> pointLight = nullptr;

		PbrGameObject(const PbrGameObject&) = delete;
		PbrGameObject& operator=(const PbrGameObject&) = delete;

		PbrGameObject(PbrGameObject&&) = default;
		PbrGameObject& operator=(PbrGameObject&&) = default;

		static PbrGameObject makePointLight(float lightIntensity = 1.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

		static PbrGameObject createGameObject()
		{
			static id_t currentId = 0;

			return PbrGameObject{ currentId++ };
		}

		id_t getId() { return id; }
	private:
		id_t id;

		PbrGameObject(id_t objId) : id{ objId } {}
	};
}
