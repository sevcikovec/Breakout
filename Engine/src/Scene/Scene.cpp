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

		//std::cout << ts << std::endl;

		auto view = ecs.GetView<ScriptComponent>();
		while (view.MoveNext()){
			auto& scriptComponent = view.GetComponent<ScriptComponent>();
			if (!scriptComponent.Instance) {
				scriptComponent.InstantiateScript(&scriptComponent);

				scriptComponent.Instance->entity = Entity{ view.GetEntity(), this };
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
		aabbVisSystem->Update(ts);

	}
	void Scene::InitRenderingSystems()
	{
		renderingSystem = CreateRef<RenderingSystem>();
		renderingSystem->Init(&ecs);
		mainCameraSetupSystem = CreateRef<MainCameraSetupSystem>();
		mainCameraSetupSystem->Init(&ecs);
		aabbVisSystem = CreateRef<AABBVisualizationSystem>();
		aabbVisSystem->Init(&ecs);

		const char* vertexShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\vert.glsl";
		const char* fragmentShader = "..\\..\\..\\..\\Engine\\resources\\shaders\\frag.glsl";
		auto shader = CreateRef<Shader>(vertexShader, fragmentShader);

		auto aabbMat = CreateRef<Material>();
		aabbMat->SetShader(shader);
		aabbMat->SetProperty("color", Vec3{ .0f, 1.f, 0 });
		aabbVisSystem->SetMaterial(aabbMat);

	}
}
