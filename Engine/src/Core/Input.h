#pragma once
#include "GLFW/glfw3.h"
#include "KeyCodes.h"
#include <Core/Application.h>
namespace Engine {
	class Input {
	public:
		static bool IsKeyDown(const KeyCode key) {
			auto* window = Application::GetInstance()->GetWindow();
			auto state = glfwGetKey(window->GetGLFW(), static_cast<int32_t>(key));
			return state == GLFW_PRESS;
		}
	};
}