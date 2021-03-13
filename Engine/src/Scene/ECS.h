#pragma once
#include <vector>
#include <queue>
#include <cassert>
#include <unordered_map>
#include <memory>

//
// Inspired by https://austinmorlan.com/posts/entity_component_system/
//
namespace Engine {

	using EntityID = uint32_t;

	class IComponentPool
	{
	public:
		virtual ~IComponentPool() = default;
		virtual void EntityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentPool : public IComponentPool {
	public:
		T& CreateComponent(EntityID entityID) {
			assert(entityToIndexMap.find(entityID) == entityToIndexMap.end());

			size_t newIndex = size++;
			entityToIndexMap[entityID] = newIndex;
			indexToEntityMap[newIndex] = entityID;
			componentVector.emplace_back();
			return componentVector[newIndex];
		}

		void RemoveEntityData(EntityID entityID) {
			assert(entityToIndexMap.find(entityID) != entityToIndexMap.end());

			size_t index = entityToIndexMap[entityID];

			componentVector[index] = componentVector[size - 1];


			EntityID movedEntity = indexToEntityMap[size - 1];
			indexToEntityMap.erase(size - 1);
			indexToEntityMap[index] = entityID;

			entityToIndexMap.erase(entityID);
			entityToIndexMap[movedEntity] = index;

			--size;
		}

		T& GetComponent(EntityID entityID) {
			assert(entityToIndexMap.find(entityID) != entityToIndexMap.end());

			return componentVector[entityToIndexMap[entityID]];
		}

		T& ContainsComponent(EntityID entityID) {
			return entityToIndexMap.find(entityID) != entityToIndexMap.end();
		}

		void EntityDestroyed(EntityID entity) override {
			if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
				RemoveEntityData(entity);
			}
		}

		std::vector<T>& GetComponents() {
			return componentVector;
		}

	private:
		std::vector<T> componentVector;

		std::unordered_map<EntityID, size_t> entityToIndexMap;
		std::unordered_map<size_t, EntityID> indexToEntityMap;

		size_t size;
	};

	class ECS {
	public:
		EntityID CreateEntity() {
			return currentID++;
		}

		template<typename T>
		T& AddComponent(EntityID entity) {
			const char* typeName = typeid(T).name();
			if (componentPools.find(typeName) == componentPools.end()) {
				componentPools.insert({ typeName, std::make_shared<ComponentPool<T>>() });
			}
			return GetComponentPool<T>()->CreateComponent(entity);
		}

		void EntityDestroyed(EntityID entity) {
			for (auto const& pair : componentPools) {
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

		template<typename T>
		void RemoveComponent(EntityID entity) {
			GetComponentPool<T>()->RemoveEntityData(entity);
		}

		template<typename T>
		void GetComponent(EntityID entity) {
			return GetComponentPool<T>().GetComponent(entity);
		}

		template<typename T>
		void HasComponent(EntityID entity) {
			const char* typeName = typeid(T).name();
			if (componentPools.find(typeName) == componentPools.end()) return false;

			return GetComponentPool<T>().ContainsComponent(entity);
		}

		template<typename T>
		std::vector<T>& GetComponentIterator(){
			const char* typeName = typeid(T).name();
			componentPools.insert({ typeName, std::make_shared<ComponentPool<T>>() });
			return GetComponentPool<T>()->GetComponents();
		}

	private:
		EntityID currentID;

		std::unordered_map<const char*, std::shared_ptr<IComponentPool>> componentPools{};

		template<typename T>
		std::shared_ptr<ComponentPool<T>> GetComponentPool()
		{
			const char* typeName = typeid(T).name();

			assert(componentPools.find(typeName) != componentPools.end());

			return std::static_pointer_cast<ComponentPool<T>>(componentPools[typeName]);
		}
	};
}