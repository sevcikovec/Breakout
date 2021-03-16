#include "Systems.h"
#include "ECS.h"
#include "Scene.h"
#include "Components.h"
#include <iostream>
#include "../Mat4.h"

namespace Engine {
	void RenderingSystem::Update(float ts) {
		Renderer::Clear();
		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& tag = e.GetComponent<TagComponent>();
			auto& transform = e.GetComponent<TransformComponent>();
			auto& mesh = e.GetComponent<MeshComponent>();

			mesh.shader->SetUniformMat4("modelMat", transform.GetModelMatrix());
			Renderer::Submit(mesh.shader, mesh.vao);
		}
	}
}