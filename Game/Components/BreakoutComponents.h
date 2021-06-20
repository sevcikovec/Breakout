#pragma once
#include "Math/Vec3.h"
#include "ECS/ComponentManager.h"

struct GameManagerComponent {
	int playerLives;
	int totalScore;
	
	bool isGameOver;
};

struct BallComponent {
	float speed = 5;
};

struct BlockComponent {
	int durability = 1;
	int score = 1;
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

struct ScoreChangedEvent {
	int score = 0;
};

struct LivesChangedEvent {
	int lives = 0;
};

struct GUIReferencesComponent {
	Engine::EntityID livesTextEntity = -1;
	Engine::EntityID scoreTextEntity = -1;
};