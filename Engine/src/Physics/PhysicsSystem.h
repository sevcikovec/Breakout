#pragma once
#include "../ECS/ECS.h"
#include "DynamicsSystem.h"
#include "CollectObjectsSystem.h"

namespace Engine {
	class PhysicsSystem : public OnUpdateSystem {
	public:
		void Init(ECS* ecs) override;
		void Update(float ts);

	private:
		ECS* ecs;

		PhysicsWorld physicsWorld;

		Ref<DynamicsSystem> dynamicsSystem;
		Ref<CollectObjectsSystem> collectionSystem;
	};
}