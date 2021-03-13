#pragma once
#include <string>
#include "GLFW/glfw3.h"


namespace Engine {
	class Window {
	public:
		Window(const std::string& name);

		void OnUpdate();
	private:
		GLFWwindow* window;
	};
}