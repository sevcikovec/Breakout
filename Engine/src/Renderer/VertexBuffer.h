#pragma once
#include <cstdint>

namespace Engine {
	class VertexBuffer {
	public:
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind() const;

	private:
		uint32_t rendererID;
	};

	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;

		uint32_t GetCount() const;

	private:
		uint32_t rendererID;
		uint32_t count;
	};
}