#include "Camera.h"

namespace Engine {
	Camera::Camera(){
	}

	void Camera::SetPerspective(float fov, float width, float height, float nearClip, float farClip)
	{
		this->aspectRatio = width / height;
		this->orthographic = false;
		this->fov = fov;
		this->nearClip = nearClip;
		this->farClip = farClip;
		RecalculateProjection();
	}

	void Camera::SetOrthographic(float left, float right, float bottom, float top, float nearClip, float farClip)
	{
		this->orthographic = true;
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
		this->nearClip = nearClip;
		this->farClip = farClip;
		RecalculateProjection();
	}

	Mat4 Camera::GetProjection() const
	{
		return projection;
	}

	void Camera::RecalculateProjection()
	{
		if (orthographic) {
			projection = { 0.f };
			projection(0, 0) = 2 / (right - left);

			projection(1, 1) = 2 / (top - bottom);

			projection(2, 2) = -2 / (farClip - nearClip);

			projection(0, 3) = -(right + left) / (right - left);
			projection(0, 3) = -(top + bottom) / (top - bottom);
			projection(0, 3) = -(farClip + nearClip) / (farClip - nearClip);
			projection(3, 3) = 1;
			
		}
		else {
			float tanHalfFov = tan(fov / 2.f);

			projection = { 0.f };
			projection(0, 0) = 1.f / (aspectRatio * tanHalfFov);
			projection(1, 1) = 1.f / (tanHalfFov);
			projection(2, 2) = -(farClip + nearClip) / (farClip - nearClip);
			projection(3, 2) = -1.f;
			projection(2, 3) = -(2.f * farClip * nearClip) / (farClip - nearClip);
		}
	}
	Mat4 Camera::GetViewProjection(const Mat4& transform)
	{
		return projection * Mat4(transform).Inverse();
	}
}