#include "Text.h"

namespace Engine {

	Text::Text(Ref<Font> font) : font(font)
	{

	}

	void Text::SetText(const std::string& string, int fontSize)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		
		float currAdvanceX = 0;
		for (size_t i = 0; i < string.size(); i++)
		{
			auto currChar = string.at(i);

			const auto& entry = font->GetCharEntry(currChar);

			float scale =  font->GetScaleForSize(fontSize);

			float w = (entry.xOff1 - entry.xOff0) * scale;
			float h = (entry.yOff1 - entry.yOff0) * scale;

			float xPos = currAdvanceX;
			float yPos = 0;

			// left top
			vertices.push_back(xPos);
			vertices.push_back(yPos + h);

			vertices.push_back(entry.s0);
			vertices.push_back(entry.t0);

			// left bottom
			vertices.push_back(xPos);
			vertices.push_back(yPos);

			vertices.push_back(entry.s0);
			vertices.push_back(entry.t1);;


			// right bottom
			vertices.push_back(xPos + w);
			vertices.push_back(yPos);

			vertices.push_back(entry.s1);
			vertices.push_back(entry.t1);

			// right top
			vertices.push_back(xPos + w);
			vertices.push_back(yPos + h);

			vertices.push_back(entry.s1);
			vertices.push_back(entry.t0);


			currAdvanceX += entry.advance * scale;

			int currOffset = i * 4;

			//if (i == 0) continue;
			indices.push_back(currOffset + 0);
			indices.push_back(currOffset + 1);
			indices.push_back(currOffset + 2);

			indices.push_back(currOffset + 0);
			indices.push_back(currOffset + 2);
			indices.push_back(currOffset + 3);
		}

		if (textVA == nullptr) {
			textVA = CreateRef<VertexArray>();
			auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
			textVA->SetIndexBuffer(indexBuffer);
			auto vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size());
			vertexBuffer->SetLayout({ LayoutShaderType::Float2, LayoutShaderType::Float2 });
			textVA->SetVertexBuffer(vertexBuffer);
		}
		else {
			auto indexBuffer = textVA->GetIndexBuffer();
			indexBuffer->BufferData(indices.data(), indices.size());
		}		
	}
	Ref<VertexArray> Text::GetVAO()
	{
		return textVA;
	}
}
