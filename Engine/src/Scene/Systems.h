#pragma once
#include "Entity.h"
#include "Components.h"
#include "../Physics/PhysicsComponents.h"


namespace Engine {
	
	class OnUpdateSystem : public ISystem {
	public:
		virtual void Update(float ts) = 0;
	};

	class RenderingSystem : public OnUpdateSystem {
	public:
		void Update(float ts) override;
	};

	class MainCameraSetupSystem : public OnUpdateSystem {
	public:
		void Update(float ts) override;
	};

	class AABBVisualizationSystem : public OnUpdateSystem {
	public:
		void Update(float ts) override;
		void SetMaterial(Ref<Material> material);

	private:
		Ref<Material> material;
	};
}