#pragma once
#include <cstdint>
#include <initializer_list>
#include <vector>
#include "../Utils.h"
#include <cassert>
#include <GL\glew.h>

namespace Engine {

	enum class LayoutShaderType {
		none = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Bool
	};

	static GLenum LayoutShaderTypeToGLBaseType(LayoutShaderType type)
	{
		switch (type)
		{
		case LayoutShaderType::Float:
		case LayoutShaderType::Float2:
		case LayoutShaderType::Float3:
		case LayoutShaderType::Float4:
			return GL_FLOAT;
		case LayoutShaderType::Int: 
			return GL_INT;
		case LayoutShaderType::Bool:
			return GL_BOOL;
		}
		
		assert(true);
		return 0;
	}

	static size_t LayoutShaderTypeToElementSize(LayoutShaderType type)
	{
		switch (type)
		{
		case LayoutShaderType::Float:
		case LayoutShaderType::Float2:
		case LayoutShaderType::Float3:
		case LayoutShaderType::Float4:
			return 4;
		case LayoutShaderType::Int:
			return 4;
		case LayoutShaderType::Bool:
			return 1;
		}

		assert(true);
		return 0;
	}

	static size_t LayoutShaderTypeToElementCount(LayoutShaderType type)
	{
		switch (type)
		{
		case LayoutShaderType::Float:
			return 1;
		case LayoutShaderType::Float2:
			return 2;
		case LayoutShaderType::Float3:
			return 3;
		case LayoutShaderType::Float4:
			return 4;
		case LayoutShaderType::Int:
			return 1;
		case LayoutShaderType::Bool:
			return 1;
		}

		assert(true);
		return 0;
	}

	struct BufferElement {
		uint32_t Type;
		size_t Size;
		size_t Offset;
		size_t ComponentCount;
		bool Normalized;
		BufferElement(LayoutShaderType type) : Offset(0), Normalized(false) {
			Type = LayoutShaderTypeToGLBaseType(type);
			ComponentCount = LayoutShaderTypeToElementCount(type);
			Size = LayoutShaderTypeToElementCount(type) * LayoutShaderTypeToElementSize(type);
		}
	};

	class BufferLayout {
	public:
		BufferLayout() : stride(0) {}

		BufferLayout(std::initializer_list<BufferElement> elements) : elements(elements) {
			CalculateOffsetAndStride();
		}

		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

		size_t GetStride() const { return stride; }

	private:
		void CalculateOffsetAndStride() {
			size_t offset = 0;
			stride = 0;
			for (auto& element : elements) {
				element.Offset = offset;
				offset += element.Size;
				stride += element.Size;
			}
		}

		std::vector<BufferElement> elements;
		size_t stride;
	};


	class VertexBuffer {
	public:
		VertexBuffer(float* vertices, uint32_t size);
		void BufferData(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind() const;

		const BufferLayout& GetLayout() const { return this->layout; };

		void SetLayout(const BufferLayout& layout) { this->layout = layout; }

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	private:
		uint32_t rendererID;
		BufferLayout layout;
	};
	
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		void BufferData(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;

		uint32_t GetCount() const;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	private:
		uint32_t rendererID;
		uint32_t count;
	};
}