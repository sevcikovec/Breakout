#include "Systems.h"
#include "ECS.h"
#include "Scene.h"
#include "Components.h"
#include <iostream>

namespace Engine {
	void RenderingSystem::Update(float ts) {
		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& tag = e.GetComponent<TagComponent>();
			auto& transform = e.GetComponent<TransformComponent>();
			auto& mesh = e.GetComponent<MeshComponent>();

			Renderer::Submit(mesh.shader, mesh.vao);
		}
	}
}