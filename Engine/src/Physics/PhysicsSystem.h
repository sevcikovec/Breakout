#pragma once
#include "../ECS/ECS.h"
#include "DynamicsSystem.h"
#include "CollectObjectsSystem.h"
#include "BroadphaseSystem.h"
#include "NarrowphaseSystem.h"
#include "CollisionReactionResolveSystem.h"
#include "CollisionEventDispatcherSystem.h"

namespace Engine {
	class PhysicsSystem : public OnUpdateSystem {
	public:
		void Init(ECS* ecs) override;
		void Update(float ts);

		void RegisterOnCollisionListenerSystem(Ref<ACollisionListenerSystem> onCollisionSystem);

	private:
		float currentTime = 0.f;
		ECS* ecs;

		PhysicsWorld physicsWorld;

		Ref<DynamicsSystem> dynamicsSystem;
		Ref<CollectObjectsSystem> collectionSystem;
		Ref<BroadphaseSystem> broadphaseSystem;
		Ref<NarrowphaseSystem> narrowphaseSystem;
		Ref<CollisionReactionResolveSystem> reactionResolveSystem;
		Ref<CollisionEventDispatcherSystem> eventsSystem;
	};
}