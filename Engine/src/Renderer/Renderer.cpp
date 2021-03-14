#include "Renderer.h"
#include <iostream>

namespace Engine {
	void Renderer::Submit(const Shader& shader, const VertexBuffer& vb, const IndexBuffer& ib)
	{
		shader.Bind();
		vb.Bind();
		ib.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			nullptr	            // array buffer offset
		);

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
		
	}
	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}