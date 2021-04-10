#pragma once
#include "../ECS/ECS.h"
#include "DynamicsSystem.h"

namespace Engine {
	class PhysicsSystem : public OnUpdateSystem {
	public:
		void Init(ECS* ecs);
		void Update(float ts);

	private:
		ECS* ecs;

		Ref<DynamicsSystem> dynamicsSystem;
	};
}