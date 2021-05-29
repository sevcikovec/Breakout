#include "Entity.h"

namespace Engine {

	Entity::Entity(const EntityID& id, ECS* ecs) : id(id), ecs(ecs) {
		
	}
}