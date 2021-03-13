#include <iostream>
#include "Core/Application.h"
#include "Breakout.h"
#include "BreakoutScene.h"

int main() {
	Breakout breakout;

	breakout.AddScene(new BreakoutScene());

	breakout.Run();

	return 0;
}