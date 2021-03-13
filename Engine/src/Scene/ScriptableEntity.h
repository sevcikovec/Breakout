#pragma once
#include "Entity.h"

namespace Engine {
	class ScriptableEntity {
	public:
		template<typename T>
		T& GetComponent()
		{
			return entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float deltaTime) {}
	private:
		//Entity entity;
		friend class Scene;
	};
}