#include <iostream>
#include "Scene.h"
#include "Components.h"
#include "../Renderer/Camera.h"
#include "../Renderer/UniformBuffer.h"

namespace Engine {
		
	Scene::Scene(const std::string& name) : name(name)
	{
		InitRenderingSystems();
		InitPhysicsSystems();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(ecs.CreateEntity(), &ecs);
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.name = name;

		return entity;
	}

	Entity Scene::GetEntity(EntityID entityID)
	{
		return { entityID, &ecs };
	}


	void Scene::OnUpdate(float ts)
	{

		//std::cout << 1/ts << std::endl;
		physicsSystem->Update(ts);
		
		for (auto& system : systems) {
			system->Update(ts);
		}
		
		// destroy entities
		{
			auto destroyView = ecs.GetView<DestroyTag>();
			while (destroyView.MoveNext()) {
				entitiesToDestroy.push_back(destroyView.GetEntity());
			}

			for (auto& entity : entitiesToDestroy) {
				ecs.EntityDestroyed(entity);
			}
			entitiesToDestroy.clear();
		}

		mainCameraSetupSystem->Update(ts);
		lightsSystem->Update(ts);
		renderingSystem->Update(ts);
		guiRenderingSystem->Update(ts);
		aabbVisSystem->Update(ts);

	}
	void Scene::InitRenderingSystems()
	{
		renderingSystem = CreateRef<RenderingSystem>();
		renderingSystem->Init(&ecs);
		mainCameraSetupSystem = CreateRef<MainCameraSetupSystem>();
		mainCameraSetupSystem->Init(&ecs);
		lightsSystem = CreateRef<LightsSystem>();
		lightsSystem->Init(&ecs);
		aabbVisSystem = CreateRef<AABBVisualizationSystem>();
		aabbVisSystem->Init(&ecs);
		guiRenderingSystem = CreateRef<GuiRenderingSystem>();
		guiRenderingSystem->Init(&ecs);


		auto shader = Shader::GetShader("default");

		auto aabbMat = CreateRef<Material>();
		aabbMat->SetShader(shader);
		aabbMat->SetProperty("color", Vec3{ .0f, 1.f, 0 });
		aabbVisSystem->SetMaterial(aabbMat);

	}
	void Scene::InitPhysicsSystems()
	{
		physicsSystem = CreateRef<PhysicsSystem>();
		physicsSystem->Init(&ecs);
	}
}
