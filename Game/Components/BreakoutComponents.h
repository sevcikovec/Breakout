#pragma once
#include "Math/Vec3.h"
struct BallComponent {
	float speed = 5;
};

struct BlockComponent {
	int durability = 1;

};

struct OuterEdgeComponent {

};

struct PlayerComponent {
	float moveSpeed = 5;
	float radius = 4;
	Engine::Vec3 movementDelta = Engine::Vec3(0);
};

struct DestroyOnLeavingCircleTag {

};