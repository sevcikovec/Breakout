#pragma once
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"


namespace Engine {
	class Renderer {
	public:
		static void Submit(const Ref<Shader> shader, const Ref<VertexArray> va);

		static void Clear();
	};
}