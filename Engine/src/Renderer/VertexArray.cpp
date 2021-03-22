#include "VertexArray.h"
#include <GL\glew.h>

namespace Engine {

	VertexArray::VertexArray() : vertexBufferIndex(0)
	{
		glGenVertexArrays(1, &rendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &rendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(rendererID);
		if (vertexBuffer) vertexBuffer->Bind();
		if (indexBuffer) indexBuffer->Bind();
	}

	void VertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		Bind();
		vb->Bind();
		vertexBuffer = vb;
		uint32_t index = 0;

		auto& layout = vb->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.ComponentCount,
				element.Type,
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		indexBuffer = ib;
		glBindVertexArray(rendererID);
		indexBuffer->Bind();
	}

	Ref<VertexArray> VertexArray::Create() {
		return CreateRef<VertexArray>();
	}
}