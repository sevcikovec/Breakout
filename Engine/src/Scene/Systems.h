#pragma once
#include "Entity.h"
#include "Components.h"

namespace Engine {
	
	template<typename... C>
	class BaseSystem : public System<C...> {
	public:
		void SetContext(Scene* scene) {
			this->scene = scene;
		}
	protected:
		Scene* scene = nullptr;
	};


	class RenderingSystem : public BaseSystem<TransformComponent, MeshComponent> {
	public:
		void Update(float ts) override;
	};

	class MainCameraSetupSystem : public BaseSystem<TransformComponent, CameraComponent> {
	public:
		void Update(float ts) override;
	};
}