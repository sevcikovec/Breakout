#pragma once
#include <string>
#include "../VertexArray.h"
#include "FontLoader.h"

namespace Engine {
	class Text {
	public:
		Text(Ref<Font> font);
		void SetText(const std::string& string, int fontSize);
		
		Ref<VertexArray> GetVAO();
	private:
		std::string text;
		Ref<VertexArray> textVA = nullptr;
		Ref<Font> font;
	};
}