#pragma once
#include <iostream>
#include "Scene/ScriptableEntity.h"

class TestScript : public Engine::ScriptableEntity {
public:
	void OnCreate() override {
		printAfterSeconds = 3.f;
		totalTimer = 0.f;
		currentTimer = printAfterSeconds;
		std::cout << "initialized Test Script to log every " << printAfterSeconds << " seconds" << std::endl;
	}

	void OnUpdate(float deltaTime) override {
		currentTimer -= deltaTime;
		totalTimer += deltaTime;
		if (currentTimer <= 0) {
			currentTimer = printAfterSeconds;
			std::cout << "completed count of " << printAfterSeconds << " seconds" << "(" << (totalTimer) << "s total)" << std::endl;
		}
	}
private:
	float currentTimer;
	float printAfterSeconds;
	float totalTimer;
};