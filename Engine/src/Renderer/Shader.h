#pragma once
#include <string>
#include <GL\glew.h>
#include "../Math/Mat4.h"
#include "../../Utils.h"
#include <unordered_map>

namespace Engine {

	class Shader {
	public:
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

		void Bind() const;

		template<typename T>
		void SetUniform(const std::string& name, T&);

		void SetUniformFloat4(const std::string& name, const float& x, const float& y, const float& z, const float& w);

		void SetUniformFloat3(const std::string& name, Vec3& value);

		void SetUniformMat4(const std::string& name, Mat4& value);

		template<>
		void SetUniform<Vec3>(const std::string& name, Vec3& value) {
			SetUniformFloat3(name, value);
		}
		template<>
		void SetUniform<Mat4>(const std::string& name, Mat4& value) {
			SetUniformMat4(name, value);
		}

		static Ref<Shader> CreateShader(const std::string& name, const std::string& vertexFilePath, const std::string& fragmentFilePath);
		static Ref<Shader> GetShader(const std::string& name);

	private:
		bool Compile(const std::string& code, GLenum shaderType);
		uint32_t programID;
		std::string ReadFile(const std::string& filepath);

		static std::unordered_map<std::string, Ref<Shader>> shaders;
	};

}