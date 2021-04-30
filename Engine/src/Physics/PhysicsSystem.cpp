#include "PhysicsSystem.h"

namespace Engine {
	void PhysicsSystem::Init(ECS* ecs)
	{
		OnUpdateSystem::Init(ecs);
		
		collectionSystem = CreateRef<CollectObjectsSystem>();
		collectionSystem->Init(ecs);
		collectionSystem->SetPhysicsWorld(&physicsWorld);

		dynamicsSystem = CreateRef<DynamicsSystem>();
		dynamicsSystem->Init(ecs);
		dynamicsSystem->SetPhysicsWorld(&physicsWorld);
		
		broadphaseSystem = CreateRef<BroadphaseSystem>();
		broadphaseSystem->Init(ecs);
		broadphaseSystem->SetPhysicsWorld(&physicsWorld);

		narrowphaseSystem = CreateRef<NarrowphaseSystem>();
		narrowphaseSystem->Init(ecs);
		narrowphaseSystem->SetPhysicsWorld(&physicsWorld);
		
		reactionResolveSystem = CreateRef<CollisionReactionResolveSystem>();
		reactionResolveSystem->Init(ecs);
		reactionResolveSystem->SetPhysicsWorld(&physicsWorld);

		eventsSystem = CreateRef<CollisionEventDispatcherSystem>();
		eventsSystem->Init(ecs);
		eventsSystem->SetPhysicsWorld(&physicsWorld);
	}

	void PhysicsSystem::Update(float ts)
	{
		currentTime += ts;
		float step = ts;
		if (currentTime >= step) {
			physicsWorld.Reset();
			this->collectionSystem->Update(step);

			this->dynamicsSystem->Update(step);

			this->broadphaseSystem->Update(step);

			this->narrowphaseSystem->Update(step);
		
			this->reactionResolveSystem->Update(step);

			this->eventsSystem->Update(step);
			currentTime = 0;
		}
	}
	void PhysicsSystem::RegisterOnCollisionListenerSystem(Ref<ACollisionListenerSystem> onCollisionSystem)
	{
		eventsSystem->AddListener(onCollisionSystem);
	}
}