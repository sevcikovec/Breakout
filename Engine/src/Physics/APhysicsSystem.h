#pragma once
#include "PhysicsWorld.h"
#include "../Scene/Systems.h"

namespace Engine {
	class APhysicsSystem : public OnUpdateSystem {
	public:
		void SetPhysicsWorld(PhysicsWorld* pWorld) {
			this->pWorld = pWorld;
		}
	protected:
		PhysicsWorld* pWorld = nullptr;
	};
}