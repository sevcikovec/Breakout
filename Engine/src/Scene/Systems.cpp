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
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		GLenum err;


		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& tag = e.GetComponent<TagComponent>();
			auto& transform = e.GetComponent<TransformComponent>();
			auto& mesh = e.GetComponent<MeshComponent>();
			
			mesh.material->GetShader()->Bind();
			mesh.material->GetShader()->SetUniformMat4("modelMat", transform.GetTransformMatrix());
			mesh.vao->Bind();
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << err << std::endl;
			}

			Renderer::Submit(mesh.material, mesh.vao);
		}
	}

	void AABBVisualizationSystem::Update(float ts) {
		// TODO refactor 
		glDisable (GL_DEPTH_TEST);
		GLenum err;


		for (auto& entity : entities)
		{
			auto e = scene->GetEntity(entity);
			auto& tag = e.GetComponent<TagComponent>();
			auto& transform = e.GetComponent<TransformComponent>();
			auto& aabb = e.GetComponent<AABB>();

			this->material->GetShader()->Bind();
			this->material->GetShader()->SetUniformMat4("modelMat", transform.GetTransformMatrix());

			this->material->BindProperties();
			glLineWidth(1); 
			
			// front side
			glBegin(GL_LINE_LOOP);
			glVertex3f(aabb.xMin, aabb.yMin, aabb.zMin);
			glVertex3f(aabb.xMax, aabb.yMin, aabb.zMin);
			glVertex3f(aabb.xMax, aabb.yMax, aabb.zMin);
			glVertex3f(aabb.xMin, aabb.yMax, aabb.zMin);
			glEnd();

			// back side
			glBegin(GL_LINE_LOOP);
			glVertex3f(aabb.xMin, aabb.yMin, aabb.zMax);
			glVertex3f(aabb.xMax, aabb.yMin, aabb.zMax);
			glVertex3f(aabb.xMax, aabb.yMax, aabb.zMax);
			glVertex3f(aabb.xMin, aabb.yMax, aabb.zMax);
			glEnd();
			
			// sides
			glBegin(GL_LINES);
			glVertex3f(aabb.xMin, aabb.yMin, aabb.zMin);
			glVertex3f(aabb.xMin, aabb.yMin, aabb.zMax);

			glVertex3f(aabb.xMax, aabb.yMin, aabb.zMin);
			glVertex3f(aabb.xMax, aabb.yMin, aabb.zMax);
			
			glVertex3f(aabb.xMin, aabb.yMax, aabb.zMin);
			glVertex3f(aabb.xMin, aabb.yMax, aabb.zMax);
									   
			glVertex3f(aabb.xMax, aabb.yMax, aabb.zMin);
			glVertex3f(aabb.xMax, aabb.yMax, aabb.zMax);
			glEnd();

			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << err << std::endl;
			}
		}
	}

	void AABBVisualizationSystem::SetMaterial(Ref<Material> material) {
		this->material = material;
	}
}