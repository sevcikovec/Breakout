#pragma once
#include "../Scene/Scene.h"
#include <vector>
#include "Window.h"

namespace Engine {
	class Application {
	public:
		Application(const std::string& name);

		void Run();

		void Application::AddScene(Scene* scene);

	private:
		Window window;

		std::vector<Scene*> scenes;

		std::string name;

		bool isRunning;

		bool initialized;
	};
}