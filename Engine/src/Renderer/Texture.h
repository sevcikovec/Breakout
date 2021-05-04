#pragma once
#include<string>

namespace Engine {
	class Texture {
	public:
		Texture(unsigned char* pixels, const uint16_t width, const uint16_t height);
		Texture(const std::string& texturePath);

		void Bind();
		void Unbind();

	private:
		uint32_t textureID;
	};
}