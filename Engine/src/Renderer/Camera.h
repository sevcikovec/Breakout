#pragma once
#include "../mat3.h"
#include "../mat4.h"

namespace Engine{

	class Camera {
	public:
		Camera();

		void SetPerspective(float fov, float width, float height, float nearClip, float farClip);

		void SetOrthographic(float left, float right, float bottom, float top, float nearClip, float farClip);

		Mat4 GetProjection() const;

		Mat4 GetViewProjection(const Mat4& transform);
	private:
		void RecalculateProjection();


		Mat4 projection;

		bool orthographic;

		float nearClip;
		float farClip;
		
		// perspective
		float aspectRatio;
		float fov;

		// orthographic
		float left, right, bottom, top;
	};



}