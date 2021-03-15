#pragma once
#include <string>
#include "Entity.h"
#include "ECS.h"
#include <unordered_set>

namespace Engine {

	class Scene {
	public:
		Scene(const std::string& name);

		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		virtual void OnUpdate(float deltaTime);

	private:
		friend class Entity;

		std::string name;

		ECS ecs;

		std::unordered_set<Entity> entitiesToDestroy;
	};
}