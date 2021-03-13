#include <iostream>
#include "Scene.h"
#include "Components.h"

namespace Engine {
	Scene::Scene(const std::string& name) : name(name)
	{

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
	}


	void Scene::OnUpdate(float deltaTime)
	{
		//std::cout << deltaTime << std::endl;
		for (auto& scriptComponent : ecs.GetComponentIterator<ScriptComponent>()) {
			if (!scriptComponent.Instance) {
				scriptComponent.Instance = scriptComponent.InstantiateScript();
				// TODO set entity of the component
				//scriptComponent.Instance->entity = Entity{ entity, this };
				scriptComponent.Instance->OnCreate();
			}
			scriptComponent.Instance->OnUpdate(deltaTime);
		}
	}
}
