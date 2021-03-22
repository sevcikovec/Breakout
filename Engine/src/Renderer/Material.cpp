#include "Material.h"

namespace Engine {
	void Material::SetShader(Ref<Shader> shader)
	{
		this->shader = shader;
	}

	Ref<Shader> Material::GetShader() {
		return shader;
	}

	void Material::ClearProperties() {
		properties.clear();
	}

	void Material::BindProperties() {
		shader->Bind();
		for (auto& it : properties) {
			it.second->ApplyProperty(shader);
		}
	}
}