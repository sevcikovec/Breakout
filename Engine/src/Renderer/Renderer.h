#pragma once
#include "Shader.h"
#include "Buffers.h"
#include "VertexArray.h"
#include "UniformBuffer.h"


namespace Engine {
	class Renderer {
	public:
		static void Init();

		static void Submit(const Ref<Shader> shader, const Ref<VertexArray> va);

		static void Clear();

		static void SetCamera(CameraUBOData cameraData);

	private:
		static Unique<UniformBuffer<CameraUBOData>> cameraUBO;
	};
}