#pragma once
#include <vector>
#include <queue>
#include "ComponentManager.h"
#include "EntityManager.h"
#include "View.h"
//
// Inspired by https://austinmorlan.com/posts/entity_component_system/
//
namespace Engine {
	class ECS {
	public:
		EntityID CreateEntity() {
			return entityManager.CreateEntity();
		}

		template<typename T>
		ComponentType GetComponentType() {
			return componentManager.GetComponentType<T>();
		}

		template<typename T>
		T& AddComponent(EntityID entity) {
			T& newComponent = componentManager.AddComponent<T>(entity);

			Signature signature = entityManager.GetEntitySignature(entity);
			signature.set(componentManager.GetComponentType<T>(), true);
			entityManager.SetEntitySignature(entity, signature);

			return newComponent;
		}

		void EntityDestroyed(EntityID entity) {
			componentManager.EntityDestroyed(entity);
			entityManager.DestroyEntity(entity);
		}

		template<typename T>
		void RemoveComponent(EntityID entity) {
			componentManager.RemoveComponent<T>(entity);

			Signature signature = entityManager.GetEntitySignature(entity);
			signature.set(componentManager.GetComponentType<T>(), false);
			entityManager.SetEntitySignature(entity, signature);
		}

		template<typename T>
		T& GetComponent(EntityID entity) {
			return componentManager.GetComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity) {
			return componentManager.HasComponent<T>(entity);
		}

		template<typename T>
		std::vector<T>& GetComponentIterator() {
			return componentManager.GetComponentIterator<T>();
		}

		template<typename... T>
		View<T...> GetView() {
			return View<T...>{ &componentManager, &entityManager };
		}
	

	private:
		EntityManager entityManager;
		ComponentManager componentManager;
	};

	class ISystem {
	public:
		virtual void Init(ECS* ecs) {
			this->ecs = ecs;
		}

	protected:
		ECS* ecs;
	};
}