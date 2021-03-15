#pragma once
#include <string>
#include "Entity.h"
#include "ECS.h"
#include <unordered_set>

namespace Engine {
	class RenderingSystem : public System {
	public:
		void Update(float TS)
		{
			for (auto& entity : entities)
			{
				//auto& transform = ecs->GetComponent<TransformComponent>(entity);
				//auto& mesh = ecs->GetComponent<MeshComponent>(entity);
			}
		}
	};

	class Scene {
	public:
		Scene(const std::string& name);

		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		virtual void OnUpdate(float deltaTime);

	private:
		friend class Entity;

		std::string name;

		ECS ecs{};

		Ref<RenderingSystem> renderingSystem;

		std::unordered_set<Entity> entitiesToDestroy;
	};
}