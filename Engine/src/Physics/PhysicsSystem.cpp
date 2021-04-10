#include "PhysicsSystem.h"

namespace Engine {
	void PhysicsSystem::Init(ECS* ecs)
	{
		this->ecs = ecs;
		this->dynamicsSystem = CreateRef<DynamicsSystem>();
		this->dynamicsSystem->Init(ecs);
	}

	void PhysicsSystem::Update(float ts)
	{
		this->dynamicsSystem->Update(ts);
	}
}