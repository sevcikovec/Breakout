#include "Systems.h"
#include "ECS.h"
#include "Scene.h"
#include <iostream>
#include "../Mat4.h"
#include "../Renderer/Camera.h"
#include "../Renderer/UniformBuffer.h"

namespace Engine {

	void MainCameraSetupSystem::Update(float ts) {

		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& cameraComponent = e.GetComponent<CameraComponent>();
			auto& cameraTransform = e.GetComponent<TransformComponent>();


			if (cameraComponent.primary) {
				Renderer::SetCamera({ cameraComponent.camera->GetViewProjection(cameraTransform.GetTransformMatrix()), cameraTransform.position });
				return;
			}
		}
	}

	void RenderingSystem::Update(float ts) {
		
		Renderer::Clear();
		glEnable(GL_DEPTH_TEST);
		GLenum err;


		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& tag = e.GetComponent<TagComponent>();
			auto& transform = e.GetComponent<TransformComponent>();
			auto& mesh = e.GetComponent<MeshComponent>();
			
			mesh.shader->Bind();
			mesh.shader->SetUniformMat4("modelMat", transform.GetTransformMatrix());
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << err << std::endl;
			}

			Renderer::Submit(mesh.shader, mesh.vao);
		}
	}
}