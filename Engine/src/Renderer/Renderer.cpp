#include "Renderer.h"
#include <iostream>

namespace Engine {
	void Renderer::Submit(const Shader& shader, Ref<VertexArray> va)
	{
		shader.Bind();
		va->Bind();

		uint32_t count = va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		
	}
	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}