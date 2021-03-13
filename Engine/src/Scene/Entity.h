#pragma once
#include <map>
#include <string>
#include "ECS.h"

namespace Engine {
	class Entity {
	public:
		template<typename T>
		T& AddComponent() {
			T& component = scene->ecs.AddComponent<T>(id);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			T& component = scene->ecs.GetComponent<T>(id);
		}

		template<typename T>
		bool HasComponent()
		{
			return scene->ecs.HasComponent<T>(entity);
		}

		template<typename T>
		void RemoveComponent()
		{
			scene->ecs.RemoveComponent<T>(entity);
		}


	private:
		friend class Scene;

		Entity(const EntityID& id, Scene* scene);

		std::string name;

		EntityID id;

		Scene* scene = nullptr;
		
	};
}