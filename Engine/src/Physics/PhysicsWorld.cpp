#include "PhysicsWorld.h"

namespace Engine {
	void PhysicsWorld::Reset()
	{
		collisionObjects.clear();
		collisionPairs.clear();
	}

	void PhysicsWorld::AddColliderObject(ColliderObject object)
	{
		collisionObjects.push_back(object);
	}
}