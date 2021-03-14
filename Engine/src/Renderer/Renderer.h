#pragma once
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"


namespace Engine {
	class Renderer {
	public:
		void Submit(const Shader& shader, Ref<VertexArray> va);

		void Clear() const;
	};
}