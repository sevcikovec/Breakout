#pragma once
#include <string>
#include <GL\glew.h>

namespace Engine {

	class Shader {
	public:
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

		void Bind();

		void SetUniformFloat4(const std::string& name, const float& x, const float& y, const float& z, const float& w);

	private:
		bool Compile(const std::string& code, GLenum shaderType);
		uint32_t programID;
		std::string ReadFile(const std::string& filepath);

	};

}