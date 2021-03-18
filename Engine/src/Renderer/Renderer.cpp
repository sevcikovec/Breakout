#include "Renderer.h"

namespace Engine {

	Unique<UniformBuffer<CameraUBOData>> Renderer::cameraUBO = nullptr;

	void Renderer::Init(){
		cameraUBO = CreateUnique<UniformBuffer<CameraUBOData>>();
	}

	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray> va)
	{
		shader->Bind();
		va->Bind();

		uint32_t count = va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		
	}
	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetCamera(CameraUBOData cameraData) {
		cameraUBO->Bind(0);
		cameraUBO->SetData(cameraData);
	}
}