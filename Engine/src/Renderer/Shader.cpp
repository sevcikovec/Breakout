#include "shader.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Engine {

	Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
	{
		std::string vertex = ReadFile(vertexFilePath);
		std::string fragment = ReadFile(fragmentFilePath);

		programID = glCreateProgram();

		if (Compile(vertex, GL_VERTEX_SHADER) == true && Compile(fragment, GL_FRAGMENT_SHADER) == true) {
			std::cout << "Correctly loaded shader" << std::endl;
		}

	}

	void Shader::Bind()
	{
		glUseProgram(programID);
	}

	void Shader::SetUniformFloat4(const std::string& name, const float& x, const float& y, const float& z, const float& w)
	{
		GLint location = glGetUniformLocation(programID, name.c_str());
		glUniform4f(location, x, y, z, w);
	}

	bool Shader::Compile(const std::string& code, GLenum shaderType)
	{
		GLenum shaderID;

		shaderID = glCreateShader(shaderType);

		const GLchar* vertexCodeCstr = code.c_str();
		glShaderSource(shaderID, 1, &vertexCodeCstr, 0);

		glCompileShader(shaderID);

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shaderID);

			std::cerr << "Could not compile shader! Error: " << infoLog.data() << std::endl;
			return false;
		}

		glAttachShader(programID, shaderID);

		// link program
		glLinkProgram(programID);

		return true;
	}


	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				std::cout << "Could not read from file" << filepath << std::endl;
			}
		}
		else
		{
			std::cout << "Could not open file" << filepath << std::endl;
		}

		return result;
	}

}