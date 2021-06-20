#pragma once
#include<string>

namespace Engine {

	enum class TextureFormat {
		RGBA,
		RGB,
		RED
	};

	class Texture {
	public:
		Texture(unsigned char* pixels, const uint16_t width, const uint16_t height, TextureFormat format);
		Texture(const std::string& texturePath);

		void Bind();
		void Unbind();

	private:
		uint32_t textureID;
	};
}