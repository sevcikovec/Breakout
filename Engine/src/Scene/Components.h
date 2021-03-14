#pragma once
#include "../Vec3.h"
#include "ScriptableEntity.h"

namespace Engine {

	struct TagComponent {
		std::string name;
	};

	struct TransformComponent{
		Vec3 position{0.f};
		Vec3 rotation{ 0.f };
		Vec3 scale{ 0.f };
	};

	struct RendererComponent {

	};


	struct ScriptComponent {
		ScriptableEntity* Instance = nullptr;

		void (*InstantiateScript)(ScriptComponent*);
		void (*DestroyScript)(ScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = [](ScriptComponent* nsc) { nsc->Instance = static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](ScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}