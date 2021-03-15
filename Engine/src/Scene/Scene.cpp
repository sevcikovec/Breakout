#include <iostream>
#include "Scene.h"
#include "Components.h"

namespace Engine {
		
	Scene::Scene(const std::string& name) : name(name)
	{
		InitRenderingSystem();
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


		renderingSystem->Update(ts);

	}
	void Scene::InitRenderingSystem()
	{
		renderingSystem = ecs.RegisterSystem<RenderingSystem>();
		Signature signature;
		signature.set(ecs.GetComponentType<TransformComponent>());
		signature.set(ecs.GetComponentType<MeshComponent>());
		ecs.SetSystemSignature<RenderingSystem>(signature);

		renderingSystem->SetContext(this);
	}
}
