#pragma once
#include "../Scene/Entity.h"
#include "../Scene/Components.h"
#include "../Math/Vec3.h"
#include "PhysicsComponents.h"
#include <limits> 
namespace Engine {
	enum ColliderType {
		sphere,
		box,
		arch
	};

	struct AABB {
		AABB(AABB_local localAABB, Mat4 transform) {
			std::vector<Vec4> points(8);
			points.resize(8);

			points[0] = Multiply(transform, Vec4{ localAABB.xMin, localAABB.yMin, localAABB.zMin, 1 });
			points[1] = Multiply(transform, Vec4{ localAABB.xMin, localAABB.yMin, localAABB.zMax, 1 });
			points[2] = Multiply(transform, Vec4{ localAABB.xMin, localAABB.yMax, localAABB.zMax, 1 });
			points[3] = Multiply(transform, Vec4{ localAABB.xMin, localAABB.yMax, localAABB.zMin, 1 });
			points[4] = Multiply(transform, Vec4{ localAABB.xMax, localAABB.yMax, localAABB.zMax, 1 });
			points[5] = Multiply(transform, Vec4{ localAABB.xMax, localAABB.yMin, localAABB.zMax, 1 });
			points[6] = Multiply(transform, Vec4{ localAABB.xMax, localAABB.yMax, localAABB.zMin, 1 });
			points[7] = Multiply(transform, Vec4{ localAABB.xMax, localAABB.yMin, localAABB.zMin, 1 });

			xMin = FLT_MAX;
			yMin = FLT_MAX;
			zMin = FLT_MAX;

			xMax = -FLT_MAX;
			yMax = -FLT_MAX;
			zMax = -FLT_MAX;
			for (size_t i = 0; i < 8; i++)
			{
				xMin = std::min(points[i].x, xMin);
				yMin = std::min(points[i].y, yMin);
				zMin = std::min(points[i].z, zMin);

				xMax = std::max(points[i].x, xMax);
				yMax = std::max(points[i].y, yMax);
				zMax = std::max(points[i].z, zMax);
			}
		}

		float xMin, yMin, zMin;
		float xMax, yMax, zMax;

		static bool IsOverlapping(const AABB& a, const AABB& b) {
			return (a.xMin <= b.xMax && a.xMax >= b.xMin) &&
				(a.yMin <= b.yMax && a.yMax >= b.yMin) &&
				(a.zMin <= b.zMax && a.zMax >= b.zMin);
		}

	};

	struct CollisionPair {
		size_t ColliderObjectAIndex;
		size_t ColliderObjectBIndex;
		Vec3 collisionPoint;
		Vec3 collisionNormal;
	};

	struct PossibleCollisionPair
	{
		size_t ColliderObjectAIndex;
		size_t ColliderObjectBIndex;
	};

	struct ColliderObject
	{
		EntityID entity;
		AABB aabb;
		TransformComponent transform;
		ColliderType colliderType;
	};
}