#pragma once
#include <GL\glew.h>
#include "../Math/Mat4.h"

namespace Engine {
	struct CameraUBOData {
		Mat4 viewProjection;
		Vec3 position;
	};

	template<typename T>
	class UniformBuffer
	{
	public:
		UniformBuffer() {
			glGenBuffers(1, &bufferID);
			glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		~UniformBuffer() {
			glDeleteBuffers(1, &bufferID);
		}

		void Bind(GLuint index){
			glBindBufferBase(GL_UNIFORM_BUFFER, index, bufferID);
		}
		
		template<typename T>
		void SetData(T data){			
			glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	private:
		GLuint bufferID;
	};
}