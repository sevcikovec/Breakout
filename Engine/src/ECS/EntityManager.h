#pragma once
#include "ComponentManager.h"
namespace Engine {

	class EntityManager {
	public:
		EntityID CreateEntity() {

			EntityID id = nextEntityID++;
			SetEntitySignature(id, Signature());
			return id;
		}

		void DestroyEntity(EntityID entity) {
			entitySignatures.erase(entity);
		}


		Signature GetEntitySignature(EntityID entity) {
			assert(entitySignatures.find(entity) != entitySignatures.end());

			return entitySignatures[entity];
		}

		void SetEntitySignature(EntityID entity, Signature signature) {
			entitySignatures[entity] = signature;
		}


	private:
		EntityID nextEntityID{};
		std::unordered_map<EntityID, Signature> entitySignatures{};
	};
}