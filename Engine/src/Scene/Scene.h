#pragma once
#include <string>
#include <unordered_set>
#include "ECS.h"
#include "Systems.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/UniformBuffer.h"

namespace Engine {
	class Scene {
	public:
		Scene(const std::string& name);

		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		Entity GetEntity(EntityID entityID);

		virtual void OnUpdate(float deltaTime);

		template<typename T>
		void RegisterSystem() {
			auto system = ecs.RegisterSystem<T>();
			system->SetContext(this);
			systems.push_back(system);
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

		std::vector<Ref<ISystem>> systems;
	};
}