#include <iostream>
#include "Scene.h"
#include "Components.h"
#include "../Renderer/Camera.h"
#include "../Renderer/UniformBuffer.h"

namespace Engine {
		
	Scene::Scene(const std::string& name) : name(name)
	{
		InitRenderingSystems();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(ecs.CreateEntity(), this);
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.name = name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entitiesToDestroy.insert(entity);
	}

	Entity Scene::GetEntity(EntityID entityID)
	{
		return { entityID, this };
	}


	void Scene::OnUpdate(float ts)
	{

		//std::cout << deltaTime << std::endl;
		for (auto& scriptComponent : ecs.GetComponentIterator<ScriptComponent>()) {
			if (!scriptComponent.Instance) {
				scriptComponent.InstantiateScript(&scriptComponent);
				// TODO set entity of the component
				//scriptComponent.Instance->entity = Entity{ entity, this };
				scriptComponent.Instance->OnCreate();
			}
			scriptComponent.Instance->OnUpdate(ts);
		}

		

		// destroy entities
		for (auto& entity : entitiesToDestroy){
		
			if (ecs.HasComponent<ScriptComponent>(entity.id)) 
			{
				auto& script = ecs.GetComponent<ScriptComponent>(entity.id);
				script.Instance->OnDestroy();
				script.DestroyScript(&script);
			}

			ecs.EntityDestroyed(entity.id);
		}
		entitiesToDestroy.clear();

		
		for (auto& system : systems) {
			system->Update(ts);
		}
		
		mainCameraSetupSystem->Update(ts);
		renderingSystem->Update(ts);

	}
	void Scene::InitRenderingSystems()
	{
		renderingSystem = ecs.RegisterSystem<RenderingSystem>();
		renderingSystem->SetContext(this);

		mainCameraSetupSystem = ecs.RegisterSystem<MainCameraSetupSystem>();
		mainCameraSetupSystem->SetContext(this);
	}
}
