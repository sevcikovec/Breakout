#pragma once
#include "EntityManager.h"

namespace Engine {

	template<typename... Components>
	class View {	
	public:
		View(ComponentManager* componentManager, EntityManager* entityManager) {
			this->componentManager = componentManager;
			this->entityManager = entityManager;


			this->currentIndex = -1;
			InitPrimaryComponent<Components...>();
			this->signature = this->componentManager->GetSignature<Components...>();
		}

		bool MoveNext() {
			while (mainComponentPool->Size() > ++this->currentIndex) {
				currentEntityID = mainComponentPool->GetEntityAtIndex(this->currentIndex);
				auto currentEntSignature = entityManager->GetEntitySignature(currentEntityID);
				if ((signature & currentEntSignature) == signature) {
					return true;
				}
			}
			return false;
		}


		template<typename T>
		T& GetComponent() {
			return componentManager->GetComponent<T>(currentEntityID);
		}
		
		EntityID GetEntity() {
			return currentEntityID;
		}

	private:

		template<typename PrimaryComponent, typename... Others>
		void InitPrimaryComponent() {
			mainComponent = componentManager->GetComponentType<PrimaryComponent>();
			mainComponentPool = componentManager->GetComponentPool<PrimaryComponent>();
		}




		int currentIndex;
		EntityID currentEntityID;

		Ref<IComponentPool> mainComponentPool;

		ComponentManager* componentManager;
		EntityManager* entityManager;

		Signature signature;
		ComponentType mainComponent;
	};
}