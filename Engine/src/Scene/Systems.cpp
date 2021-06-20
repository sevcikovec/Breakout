#include "Systems.h"
#include "../ECS/ECS.h"
#include "Scene.h"
#include <iostream>
#include "../Mat4.h"
#include "../Renderer/Camera.h"
#include "../Renderer/UniformBuffer.h"
#include "../Core/Input.h"
#include "../Physics/CollisionStructs.h"

namespace Engine {

	void MainCameraSetupSystem::Update(float ts) {

		auto view = ecs->GetView<CameraComponent, TransformComponent>();
		while (view.MoveNext())
		{
			auto& cameraComponent = view.GetComponent<CameraComponent>();
			auto& cameraTransform = view.GetComponent<TransformComponent>();


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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		GLenum err;

		auto view = ecs->GetView<MeshComponent, TransformComponent, TagComponent>();
		while (view.MoveNext())
		{
			auto& tag = view.GetComponent<TagComponent>();
			auto& transform = view.GetComponent<TransformComponent>();
			auto& mesh = view.GetComponent<MeshComponent>();
			
			mesh.material->GetShader()->Bind();
			mesh.material->GetShader()->SetUniformMat4("modelMat", transform.GetTransformMatrix());
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << err << std::endl;
			}

			Renderer::Submit(mesh.material, mesh.vao);
		}
	}

	void LightsSystem::Update(float ts) {
		
		int lightsCount = 0;
		auto view = ecs->GetView<LightComponent, TransformComponent>();
		while (view.MoveNext()) {
			auto& transform = view.GetComponent<TransformComponent>();
			auto& light = view.GetComponent<LightComponent>();
			data.lights[lightsCount] = { transform.position, light.ambient, light.diffuse, light.specular };
			lightsCount++;
		}
		data.currentLights = lightsCount;
		Renderer::SetLights(data);
	}

	void AABBVisualizationSystem::Update(float ts) {

		if (!Input::IsKeyDown(KeyCode::B)) {
			return;
		}


		// TODO refactor 
		glDisable (GL_DEPTH_TEST);
		GLenum err;

		auto view = ecs->GetView<AABB_local, TransformComponent, TagComponent>();
		while (view.MoveNext())
		{
			auto& tag = view.GetComponent<TagComponent>();
			auto& transform = view.GetComponent<TransformComponent>();
			auto& aabb_local = view.GetComponent<AABB_local>();

			auto aabb = AABB(aabb_local, transform.GetTransformMatrix());

			this->material->GetShader()->Bind();
			//this->material->GetShader()->SetUniformMat4("modelMat", transform.GetTransformMatrix());
			this->material->GetShader()->SetUniformMat4("modelMat", Mat4(1));

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
	void GuiRenderingSystem::Update(float ts)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		GLenum err;

		auto view = ecs->GetView<TextComponent, RectTransform>();
		while (view.MoveNext())
		{
			auto& text = view.GetComponent<TextComponent>();
			auto& transform = view.GetComponent<RectTransform>();

			if (ecs->HasComponent<CanvasComponent>(transform.parentCanvasEntity)) {
				auto& canvasComponent = ecs->GetComponent<CanvasComponent>(transform.parentCanvasEntity);
				if (ecs->HasComponents<CameraComponent, TransformComponent>(canvasComponent.cameraEntity)) {
					auto& cameraComponent = ecs->GetComponent<CameraComponent>(canvasComponent.cameraEntity);
					auto& cameraTransform = ecs->GetComponent<TransformComponent>(canvasComponent.cameraEntity);

					auto x = Multiply(cameraComponent.camera->GetViewProjection(cameraTransform.GetTransformMatrix()) * transform.GetTransformMatrix(), Vec4(0,0,0,1));
					Renderer::SetCamera({ cameraComponent.camera->GetViewProjection(cameraTransform.GetTransformMatrix()), cameraTransform.position });
				}
			}


			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << err << std::endl;
			}

			text.material->GetShader()->Bind();
			text.material->GetShader()->SetUniformMat4("modelMat", transform.GetTransformMatrix());
			Renderer::Submit(text.material, text.text->GetVAO());
		}

		glDisable(GL_BLEND);
	}
}