#pragma once
#include "Entity.h"

namespace Engine {
	class BaseSystem : public System {
	public:
		void SetContext(Scene* scene) {
			this->scene = scene;
		}
	protected:
		Scene* scene = nullptr;
	};


	class RenderingSystem : public BaseSystem {
	public:
		void Update(float ts);
	};

	class MainCameraSetupSystem : public BaseSystem {
	public:
		void Update(float ts);
	};
}