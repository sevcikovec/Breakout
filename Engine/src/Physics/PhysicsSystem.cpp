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
	}

	void PhysicsSystem::Update(float ts)
	{
		this->dynamicsSystem->Update(ts);

		this->collectionSystem->Update(ts);
	}
}