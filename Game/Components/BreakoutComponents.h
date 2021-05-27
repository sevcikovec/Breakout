#pragma once
#include "Math/Vec3.h"
struct BallComponent {

};

struct BlockComponent {
	int durability = 1;

};

struct PlayerComponent {
	float moveSpeed = 5;
	float radius = 4;
	Engine::Vec3 movementDelta = Engine::Vec3(0);
};