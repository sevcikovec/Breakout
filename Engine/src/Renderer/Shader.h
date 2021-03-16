#pragma once
#include <string>
#include <GL\glew.h>
#include "../Math/Mat4.h"

namespace Engine {

	class Shader {
	public:
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

		void Bind() const;

		void SetUniformFloat4(const std::string& name, const float& x, const float& y, const float& z, const float& w);

		void SetUniformMat4(const std::string& name, Mat4& value);


	private:
		bool Compile(const std::string& code, GLenum shaderType);
		uint32_t programID;
		std::string ReadFile(const std::string& filepath);

	};

}