#include "PhysicsSystem.h"

namespace Engine {
	void PhysicsSystem::Init(ECS* ecs)
	{
		OnUpdateSystem::Init(ecs);


		this->dynamicsSystem = CreateRef<DynamicsSystem>();
		this->dynamicsSystem->Init(ecs);
		
		this->collectionSystem = CreateRef<CollectObjectsSystem>();
		this->collectionSystem->Init(ecs);
		this->collectionSystem->SetPhysicsWorld(&physicsWorld);
		
		this->broadphaseSystem = CreateRef<BroadphaseSystem>();
		this->broadphaseSystem->Init(ecs);
		this->broadphaseSystem->SetPhysicsWorld(&physicsWorld);

		this->narrowphaseSystem = CreateRef<NarrowphaseSystem>();
		this->narrowphaseSystem->Init(ecs);
		this->narrowphaseSystem->SetPhysicsWorld(&physicsWorld);

		this->eventsSystem = CreateRef<CollisionEventsSystem>();
		this->eventsSystem->Init(ecs);
		this->eventsSystem->SetPhysicsWorld(&physicsWorld);
	}

	void PhysicsSystem::Update(float ts)
	{
		physicsWorld.Reset();
		this->dynamicsSystem->Update(ts);

		this->collectionSystem->Update(ts);

		this->broadphaseSystem->Update(ts);

		this->narrowphaseSystem->Update(ts);
		
		this->eventsSystem->Update(ts);
	}
}