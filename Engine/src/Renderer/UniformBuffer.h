#pragma once
#include <GL\glew.h>
#include "../Math/Mat4.h"

namespace Engine {
	struct CameraUBOData {
		Mat4 viewProjection;
		Vec3 position;
	};

	class AUniformBuffer {
	public:
		AUniformBuffer(size_t size) {
			glGenBuffers(1, &bufferID);
			glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
			glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		~AUniformBuffer() {
			glDeleteBuffers(1, &bufferID);
		}

		void Bind(GLuint index) {
			glBindBufferBase(GL_UNIFORM_BUFFER, index, bufferID);
		}

	protected:
		GLuint bufferID;

	};

	template<typename T>
	class UniformBuffer : public AUniformBuffer
	{
	public:	
		UniformBuffer() : AUniformBuffer(sizeof(T))
		{
		}

		void SetData(const T& data) {			
			glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	};
}