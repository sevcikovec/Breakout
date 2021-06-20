#include "Renderer.h"

namespace Engine {

	Unique<UniformBuffer<CameraUBOData>> Renderer::cameraUBO = nullptr;
	Unique<LightDataUBO> Renderer::lightsUBO = nullptr;
	Unique<Texture> Renderer::whiteTexture = nullptr;

	void Renderer::Init(){
		cameraUBO = CreateUnique<UniformBuffer<CameraUBOData>>();
		lightsUBO = CreateUnique<LightDataUBO>();
		
		std::vector<unsigned char> whiteData{ (unsigned char)255, (unsigned char)255, (unsigned char)255,(unsigned char)255 };
		whiteTexture = CreateUnique<Texture>(whiteData.data(), 1, 1, TextureFormat::RGBA);
	}

	void Renderer::Submit(const Ref<Material> material, const Ref<VertexArray> va)
	{
		material->GetShader()->Bind();
		material->BindProperties();

		if (material->HasTextures())
			material->BindTextures();
		else
			whiteTexture->Bind();
		
		va->Bind();

		uint32_t count = va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		
		if (material->HasTextures())
			material->UnbindTextures();
		else
			whiteTexture->Unbind();
	}
	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetCamera(const CameraUBOData& cameraData) {
		cameraUBO->Bind(0);
		cameraUBO->SetData(cameraData);
	}

	void Renderer::SetLights(const LightUBOData& lightsData) {
		lightsUBO->Bind(1);
		lightsUBO->SetData(lightsData);
	}
}