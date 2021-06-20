#pragma once
#include "ScriptableEntity.h"
#include "../Vec3.h"
#include <../Mat4.h>
#include <../Renderer/Camera.h>
#include <../Renderer/Material.h>
#include <../Renderer/VertexArray.h>
#include "../Math/Coordinates.h"
#include "../Renderer/Text/Text.h"

namespace Engine {

	struct DestroyTag{};

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

		static TransformComponent GetArchTransform(float angle, float radius, float y) {
			TransformComponent transform;
			transform.position.y = y;
			transform.rotation.y = angle;

			transform.position = CylindricalToCartesian({ radius, transform.position.y, transform.rotation.y });
			return transform;
		}
	};

	struct MeshComponent {
		Ref<VertexArray> vao;
		Ref<Material> material;
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

	struct LightComponent {
		Vec3 ambient;
		Vec3 diffuse;
		Vec3 specular;
	};

	struct TextComponent {
		Ref<Text> text;
		Ref<Material> material;
		Ref<Camera> camera;
	};

	struct CanvasComponent {
		EntityID cameraEntity;
	};

	struct RectTransform {
		EntityID parentCanvasEntity;

		Vec3 position{ 0.f };
		Vec2 size{ 0.f, 0.f };
		Vec3 rotation{ 0.f };
		Vec3 scale{ 1.f };

		Mat4 GetTransformMatrix() {
			Quaternion rot = Quaternion::FromEuler(rotation);
			auto x = Mat4(Mat3::RotationMatrix(rot));
			Mat4 modelMat = Mat4::GetTranslationMatrix(position) * Mat4(Mat3::RotationMatrix(rot)) * Mat4::GetScaleMatrix(scale);

			return modelMat;
		}
	};
}