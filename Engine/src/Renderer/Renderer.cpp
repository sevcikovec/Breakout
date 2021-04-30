#include "Renderer.h"

namespace Engine {

	Unique<UniformBuffer<CameraUBOData>> Renderer::cameraUBO = nullptr;
	Unique<LightDataUBO> Renderer::lightsUBO = nullptr;

	void Renderer::Init(){
		cameraUBO = CreateUnique<UniformBuffer<CameraUBOData>>();
		lightsUBO = CreateUnique<LightDataUBO>();
	}

	void Renderer::Submit(const Ref<Material> material, const Ref<VertexArray> va)
	{
		material->GetShader()->Bind();
		material->BindProperties();
		va->Bind();

		uint32_t count = va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		
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