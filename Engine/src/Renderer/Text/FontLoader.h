#pragma once
#include <unordered_map>
#include"../../Utils.h"
#include "../Texture.h"

namespace Engine {
	struct FontEntry {
		unsigned char character;
		float s0, t0, s1, t1; // coordinates in texture
		float advance;
		float xOff0, xOff1, yOff0, yOff1;
		float x0, x1, y0, y1;
	};

	class Font {
	public:
		Font(Ref<Texture> atlas, FontEntry* fontEntries, int numOfEntries);

		FontEntry& GetCharEntry(unsigned char c);

		float GetScaleForSize(int fontSize);

		Ref<Texture> GetAtlasTexture();
	private:
		friend class FontLoader;

		std::unordered_map<unsigned char, FontEntry> entries;
		Ref<Texture> fontAtlas;

		// saved value of scale for 1 pixel
		float maxHeightDistance;
	};

	class FontLoader {
	public:
		static Ref<Font> LoadFont(const char* fontPath);
	private:

	};
}