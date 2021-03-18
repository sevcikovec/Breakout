#pragma once
#include <vector>
#include <queue>
#include <cassert>
#include <unordered_map>
#include <memory>
#include <bitset>
#include <set>
#include "../Utils.h"
//
// Inspired by https://austinmorlan.com/posts/entity_component_system/
//
namespace Engine {

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENT_COUNT = 32;

	using EntityID = uint32_t;

	using Signature = std::bitset<MAX_COMPONENT_COUNT>;

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

		std::vector<T>& GetComponents() {
			return componentVector;
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

	private:
		std::unordered_map<const char*, Ref<IComponentPool>> componentPools{};
		std::unordered_map<const char*, ComponentType> componentTypes{};
		ComponentType nextComponentType{};


		template<typename T>
		Ref<ComponentPool<T>> GetComponentPool()
		{
			const char* typeName = typeid(T).name();

			assert(componentPools.find(typeName) != componentPools.end());

			return std::static_pointer_cast<ComponentPool<T>>(componentPools[typeName]);
		}
	};

	class ISystem {
	public:
		std::set<EntityID> entities{};

		virtual Signature GetSignature(ComponentManager* componentSystem) = 0;
		virtual void Update(float ts) = 0;
	};

	template<typename... C>
	class System : public ISystem{
	public:
		Signature GetSignature(ComponentManager* componentSystem) override {
			return GetSignature<C...>(componentSystem);
		}
		
	private:
		template<typename... C>
		Signature GetSignature(ComponentManager* componentSystem) {
			Signature signature;
			GetSignatureRec<C...>(componentSystem, signature);
			return signature;
		};

		template<typename C1, typename C2, typename... Cx>
		void GetSignatureRec(ComponentManager* componentSystem, Signature& signature) {
			GetSignatureRec<C1>(componentSystem, signature);
			GetSignatureRec<C2, Cx...>(componentSystem, signature);
		}

		template<typename C>
		void GetSignatureRec(ComponentManager* componentSystem, Signature& signature) {
			signature.set(componentSystem->GetComponentType<C>());
		}
	};


	class SystemManager {
	public:
		template<typename T>
		Ref<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			assert(systems.find(typeName) == systems.end());

			auto system = CreateRef<T>();
			systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(EntityID entity)
		{
			for (auto const& pair : systems)
			{
				auto const& system = pair.second;

				system->entities.erase(entity);
			}
		}

		void EntitySignatureChanged(EntityID entity, Signature entitySignature)
		{
			for (auto const& pair : systems)
			{
				auto const& system = pair.second;
				auto const& type = pair.first;
				auto const& systemSignature = signatures[type];

				system->entities.erase(entity);

				// new signature matches system signature
				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->entities.insert(entity);
				}
				// new signature does not match system signature
				else
				{
					system->entities.erase(entity);
				}
			}
		}

	private:
		std::unordered_map<const char*, Ref<ISystem>> systems{};
		std::unordered_map<const char*, Signature> signatures{};
	};
	
	
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
	

	class ECS {
	public:
		EntityID CreateEntity() {
			return entityManager.CreateEntity();
		}

		template<typename SystemType>
		Ref<SystemType> RegisterSystem() {
			Ref<SystemType> system = systemManager.RegisterSystem<SystemType>();
			Ref<ISystem> baseSystem = static_cast<Ref<ISystem>>(system);
			Signature signature = baseSystem->GetSignature(&componentManager);
			systemManager.SetSignature<SystemType>(signature);
			return system;
		}

		template<typename T>
		ComponentType GetComponentType() {
			return componentManager.GetComponentType<T>();
		}

		template<typename T>
		T& AddComponent(EntityID entity) {
			T& newComponent = componentManager.AddComponent<T>(entity);

			Signature signature = entityManager.GetEntitySignature(entity);
			signature.set(componentManager.GetComponentType<T>(), true);
			entityManager.SetEntitySignature(entity, signature);

			systemManager.EntitySignatureChanged(entity, signature);

			return newComponent;
		}

		void EntityDestroyed(EntityID entity) {
			componentManager.EntityDestroyed(entity);
			systemManager.EntityDestroyed(entity);
			entityManager.DestroyEntity(entity);
		}

		template<typename T>
		void RemoveComponent(EntityID entity) {
			T& newComponent = componentManager.AddComponent<T>(entity);

			Signature signature = entityManager.GetEntitySignature(entity);
			signature.set(componentManager.GetComponentType<T>(), false);
			entityManager.SetEntitySignature(entity, signature);

			systemManager.EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(EntityID entity) {
			return componentManager.GetComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity) {
			return componentManager.HasComponent<T>(entity);
		}

		template<typename T>
		std::vector<T>& GetComponentIterator() {
			return componentManager.GetComponentIterator<T>();
		}

	private:
		SystemManager systemManager;
		EntityManager entityManager;
		ComponentManager componentManager;
	};
}