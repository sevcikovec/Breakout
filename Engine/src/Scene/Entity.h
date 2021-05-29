#pragma once
#include <map>
#include <string>
#include "../ECS/ECS.h"

namespace Engine {
	class Entity {
	public:
		Entity(const EntityID& id, ECS* ecs);

		template<typename T>
		T& AddComponent() {
			T& component = ecs->AddComponent<T>(id);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			T& component = ecs->GetComponent<T>(id);
			return component;
		}

		template<typename T>
		bool HasComponent()
		{
			return ecs->HasComponent<T>(id);
		}

		template<typename T>
		void RemoveComponent()
		{
			ecs->RemoveComponent<T>(id);
		}

		size_t operator==(const Entity& other) const {
			return id == other.id;
		}

		EntityID GetID() const {
			return id;
		}

	private:
		std::string name;

		EntityID id;

		ECS* ecs = nullptr;
	};
}

namespace std {
	template<>
	struct hash<Engine::Entity> {
		size_t operator()(const Engine::Entity& t) const
		{
			return t.GetID();
		}
	};
}