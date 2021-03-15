#include "Renderer.h"
#include <iostream>

namespace Engine {
	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray> va)
	{
		shader->Bind();
		va->Bind();

		uint32_t count = va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		
	}
	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}