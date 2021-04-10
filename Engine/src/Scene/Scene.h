#pragma once
#include <string>
#include <unordered_set>
#include "../ECS.h"
#include "Systems.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/UniformBuffer.h"
#include "../Physics/PhysicsSystem.h"

namespace Engine {
	class Scene {
	public:
		Scene(const std::string& name);

		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		Entity GetEntity(EntityID entityID);

		template<typename... T>
		View<T...> GetView() {
			return ecs.GetView<T...>();
		}

		virtual void OnUpdate(float deltaTime);

		template<typename T>
		Ref<T> RegisterSystem() {
			auto system = CreateRef<T>();
			system->Init(&ecs);
			systems.push_back(system);
			return system;
		}

	protected:
		std::string name;


	private:
		friend class Entity;
		
		ECS ecs{};

		std::unordered_set<Entity> entitiesToDestroy;

		void InitRenderingSystems();
		Ref<RenderingSystem> renderingSystem;
		Ref<MainCameraSetupSystem> mainCameraSetupSystem;
		Ref<AABBVisualizationSystem> aabbVisSystem;

		void InitPhysicsSystems();
		Ref<PhysicsSystem> physicsSystem;

		std::vector<Ref<OnUpdateSystem>> systems;
	};
}