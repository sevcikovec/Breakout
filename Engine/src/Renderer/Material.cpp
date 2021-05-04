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

	void Material::SetTexture(Ref<Texture> texture) {
		this->texture = texture;
	}

	bool Material::HasTextures() {
		return texture != nullptr;
	}

	void Material::BindTextures() {
		if (texture != nullptr) {
			texture->Bind();
		}
	}

	void Material::UnbindTextures() {
		if (texture != nullptr) {
			texture->Unbind();
		}
	}
}