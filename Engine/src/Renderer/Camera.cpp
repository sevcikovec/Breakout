#include "Camera.h"

namespace Engine {
	Camera::Camera(){
	}

	void Camera::SetPerspective(float fov, float nearClip, float farClip)
	{
		this->fov = fov;
		this->nearClip = nearClip;
		this->farClip = farClip;
		RecalculateProjection();
	}

	void Camera::SetViewport(float width, float height)
	{
		aspectRatio = width / height;
		RecalculateProjection();
	}

	Mat4 Camera::GetProjection() const
	{
		return projection;
	}

	void Camera::RecalculateProjection()
	{
		float tanHalfFov = tan(fov / 2.f);

		projection = { 0.f };
		projection(0, 0) = 1.f / (aspectRatio * tanHalfFov);
		projection(1, 1) = 1.f / (tanHalfFov);
		projection(2, 2) = -(farClip + nearClip) / (farClip - nearClip);
		projection(3, 2) = -1.f;
		projection(2, 3) = -(2.f * farClip * nearClip) / (farClip - nearClip);
	}
	Mat4 Camera::GetViewProjection(const Mat4& transform)
	{
		return projection * Mat4(transform).Inverse();
	}
}