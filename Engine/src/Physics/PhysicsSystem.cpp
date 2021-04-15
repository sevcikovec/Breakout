#include "PhysicsSystem.h"

namespace Engine {
	void PhysicsSystem::Init(ECS* ecs)
	{
		OnUpdateSystem::Init(ecs);


		dynamicsSystem = CreateRef<DynamicsSystem>();
		dynamicsSystem->Init(ecs);
		
		collectionSystem = CreateRef<CollectObjectsSystem>();
		collectionSystem->Init(ecs);
		collectionSystem->SetPhysicsWorld(&physicsWorld);
		
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
		physicsWorld.Reset();
		this->dynamicsSystem->Update(ts);

		this->collectionSystem->Update(ts);

		this->broadphaseSystem->Update(ts);

		this->narrowphaseSystem->Update(ts);
		
		this->reactionResolveSystem->Update(ts);

		this->eventsSystem->Update(ts);
	}
	void PhysicsSystem::RegisterOnCollisionListenerSystem(Ref<ACollisionListenerSystem> onCollisionSystem)
	{
		eventsSystem->AddListener(onCollisionSystem);
	}
}