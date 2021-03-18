#pragma once
#include "ScriptableEntity.h"
#include "../Vec3.h"
#include <../Mat4.h>
#include <../Renderer/Camera.h>
#include <../Renderer/Shader.h>
#include <../Renderer/VertexArray.h>

namespace Engine {

	struct TagComponent {
		std::string name;
	};

	struct TransformComponent{
		Vec3 position{0.f};
		Vec3 rotation{ 0.f };
		Vec3 scale{ 1.f };

		Mat4 GetTransformMatrix() {
			Quaternion rot = Quaternion::FromEuler(rotation);
			auto x = Mat4(Mat3::RotationMatrix(rot));
			Mat4 modelMat = Mat4::GetTranslationMatrix(position) * Mat4(Mat3::RotationMatrix(rot)) * Mat4::GetScaleMatrix(scale);
			
			return modelMat;
		}
	};

	struct MeshComponent {
		Ref<VertexArray> vao;
		Ref<Shader> shader;
	};

	struct CameraComponent {
		bool primary;
		Ref<Camera> camera;
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