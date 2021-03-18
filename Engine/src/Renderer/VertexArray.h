#pragma once
#include "Buffers.h"
#include <vector>
#include "../Utils.h"

namespace Engine {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;

		Ref<VertexBuffer> GetVertexBuffer() const { return vertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() const { return indexBuffer; }

		void SetVertexBuffer(const Ref<VertexBuffer>& vb);
		void SetIndexBuffer(const Ref<IndexBuffer>& ib);


		static Ref<VertexArray> Create();
	private:
		uint32_t rendererID;
		uint32_t vertexBufferIndex;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

	};
}