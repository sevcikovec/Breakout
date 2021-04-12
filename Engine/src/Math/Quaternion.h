#pragma once
#include "Vec3.h"
namespace Engine {
# define PI           3.14159265358979323846f  /* pi */
    static float ToRadians(float degrees) {
        return degrees * PI / 180.f;
    }

    static float ToDegrees(float radians) {
        return radians * 180.f / PI;
    }

	class Quaternion {
	public:
		Quaternion(float yaw, float pitch, float roll) {
            float cy = cos(yaw * 0.5f);
            float sy = sin(yaw * 0.5f);
            float cp = cos(pitch * 0.5f);
            float sp = sin(pitch * 0.5f);
            float cr = cos(roll * 0.5f);
            float sr = sin(roll * 0.5f);

            w = cr * cp * cy + sr * sp * sy;
            x = sr * cp * cy - cr * sp * sy;
            y = cr * sp * cy + sr * cp * sy;
            z = cr * cp * sy - sr * sp * cy;
		}
		float w, x, y, z;

        void Normalize() {
            const float n = 1.0f / sqrt(x * x + y * y + z * z + w * w);
            x *= n;
            y *= n;
            z *= n;
            w *= n;
        }

        Quaternion Normalized() const{
            Quaternion q = *this;
            q.Normalize();
            return q;
        }

        static Quaternion FromEuler(Vec3 angles) {
            return { angles.z * PI/180.f, angles.y * PI / 180.f, angles.x* PI / 180.f };
        }
	};
}