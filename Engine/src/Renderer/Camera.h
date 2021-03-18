#pragma once
#include "../mat3.h"
#include "../mat4.h"

namespace Engine{

	class Camera {
	public:
		Camera();

		void SetPerspective(float fov, float nearClip, float farClip);

		void SetViewport(float width, float height);

		Mat4 GetProjection() const;

		Mat4 GetViewProjection(const Mat4& transform);
	private:
		void RecalculateProjection();


		Mat4 projection;

		float aspectRatio;
		float fov;
		float nearClip;
		float farClip;
	};



}