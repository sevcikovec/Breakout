#pragma once
#include "Shader.h"
#include "VertexBuffer.h"


namespace Engine {
	class Renderer {
	public:
		void Submit(const Shader& shader, const VertexBuffer& vb, const IndexBuffer& ib);

		void Clear() const;
	};
}