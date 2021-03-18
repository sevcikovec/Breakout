#include "UniformBuffer.h"

namespace Engine {
/*
	template<typename T>
	UniformBuffer<T>::UniformBuffer()
	{
		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<typename T>
	UniformBuffer<T>::~UniformBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	template<typename T>
	void UniformBuffer<T>::Bind(GLuint index)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, index, bufferID);
	}
	
	template<typename T>
	void UniformBuffer<T>::SetData(T data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
		if (!data.empty())
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), 1);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}*/

}