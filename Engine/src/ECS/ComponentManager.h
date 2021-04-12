#pragma once
#include <cassert>
#include <unordered_map>
#include <bitset>
#include <set>
#include "../Utils.h"

namespace Engine {
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENT_COUNT = 32;

	using Signature = std::bitset<MAX_COMPONENT_COUNT>;
	using EntityID = uint32_t;

	class IComponentPool
	{
	public:
		virtual ~IComponentPool() = default;
		virtual void EntityDestroyed(EntityID entity) = 0;
		virtual EntityID GetEntityAtIndex(size_t index) = 0;
		virtual size_t Size() = 0;
	};

	template<typename T>
	class ComponentPool : public IComponentPool {
	public:
		T& CreateComponent(EntityID entityID) {
			if (entityToIndexMap.find(entityID) != entityToIndexMap.end()) {
				int x = 1;
			}
			assert(entityToIndexMap.find(entityID) == entityToIndexMap.end());

			componentVector.emplace_back();

			size_t newIndex = componentVector.size() - 1;
			entityToIndexMap[entityID] = newIndex;
			indexToEntityMap[newIndex] = entityID;

			return componentVector[newIndex];
		}

		void RemoveEntityData(EntityID entityID) {
			assert(entityToIndexMap.find(entityID) != entityToIndexMap.end());

			size_t index = entityToIndexMap[entityID];
			size_t lastIndex = componentVector.size() - 1;

			if (index == lastIndex) {
				indexToEntityMap.clear();
				componentVector.clear();
				entityToIndexMap.clear();
				return;
			}

			// move components so the pool remain dense
			componentVector[index] = componentVector[lastIndex];
			componentVector.pop_back();

			EntityID movedEntity = indexToEntityMap[lastIndex];
			indexToEntityMap.erase(lastIndex);
			indexToEntityMap[index] = entityID;

			entityToIndexMap.erase(entityID);
			entityToIndexMap[movedEntity] = index;
		}

		T& GetComponent(EntityID entityID) {
			assert(entityToIndexMap.find(entityID) != entityToIndexMap.end());

			return componentVector[entityToIndexMap[entityID]];
		}

		bool ContainsComponent(EntityID entityID) {
			return entityToIndexMap.find(entityID) != entityToIndexMap.end();
		}

		void EntityDestroyed(EntityID entity) override {
			if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
				RemoveEntityData(entity);
			}
		}

		EntityID GetEntityAtIndex(size_t index) override {
			assert(indexToEntityMap.find(index) != indexToEntityMap.end());
			return indexToEntityMap[index];
		}

		T& GetComponentAtIndex(size_t index) {
			assert(0 <= index && index < componentVector.size());
			return componentVector[index];
		}

		std::vector<T>& GetComponents() {
			return componentVector;
		}

		size_t Size() override{
			return componentVector.size();
		}

	private:
		std::vector<T> componentVector;

		std::unordered_map<EntityID, size_t> entityToIndexMap{};
		std::unordered_map<size_t, EntityID> indexToEntityMap{};
	};


	class ComponentManager {
	public:
		template<typename T>
		void RegisterComponent() {
			const char* typeName = typeid(T).name();
			assert(componentPools.find(typeName) == componentPools.end());

			componentPools.insert({ typeName, CreateRef<ComponentPool<T>>() });
			componentTypes.insert({ typeName, nextComponentType++ });
		}

		template<typename T>
		bool IsComponentRegistered() {
			const char* typeName = typeid(T).name();
			return componentPools.find(typeName) != componentPools.end();
		}

		template<typename T>
		T& AddComponent(EntityID entity) {
			const char* typeName = typeid(T).name();
			if (componentPools.find(typeName) == componentPools.end()) {
				componentPools.insert({ typeName, CreateRef<ComponentPool<T>>() });
				componentTypes.insert({ typeName, nextComponentType++ });
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
		T& GetComponent(EntityID entity) {
			return GetComponentPool<T>()->GetComponent(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity) {
			const char* typeName = typeid(T).name();
			if (componentPools.find(typeName) == componentPools.end()) return false;

			return GetComponentPool<T>()->ContainsComponent(entity);
		}

		template<typename T>
		std::vector<T>& GetComponentIterator() {
			const char* typeName = typeid(T).name();
			componentPools.insert({ typeName, CreateRef<ComponentPool<T>>() });
			return GetComponentPool<T>()->GetComponents();
		}

		template<typename T>
		ComponentType GetComponentType() {
			const char* typeName = typeid(T).name();
			if (!IsComponentRegistered<T>())
				RegisterComponent<T>();
			return componentTypes.find(typeName)->second;
		}

		template<typename T>
		Ref<ComponentPool<T>> GetComponentPool()
		{
			const char* typeName = typeid(T).name();

			assert(componentPools.find(typeName) != componentPools.end());

			return std::static_pointer_cast<ComponentPool<T>>(componentPools[typeName]);
		}

	public:
		template<typename... C>
		Signature GetSignature() {
			Signature signature;
			GetSignatureRec<C...>(signature);
			return signature;
		};

	private:
		template<typename C1, typename C2, typename... Cx>
		void GetSignatureRec(Signature& signature) {
			GetSignatureRec<C1>(signature);
			GetSignatureRec<C2, Cx...>(signature);
		}

		template<typename C>
		void GetSignatureRec(Signature& signature) {
			signature.set(GetComponentType<C>());
		}

	private:
		std::unordered_map<const char*, Ref<IComponentPool>> componentPools{};
		std::unordered_map<const char*, ComponentType> componentTypes{};
		ComponentType nextComponentType{};
	};
}
