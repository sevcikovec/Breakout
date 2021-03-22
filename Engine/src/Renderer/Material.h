#pragma once
#include "Shader.h"
#include "../Utils.h"
#include <vector>
#include <unordered_map>
namespace Engine {

	class IMaterialProperty {
	public:
		void virtual ApplyProperty(Ref<Shader> shader) = 0;
	};

	template<typename T>
	class MaterialProperty : public IMaterialProperty {
	public:
		MaterialProperty(const std::string& name, const T& value) {
			this->value = value;
			this->name = name;
		}

		void ApplyProperty(Ref<Shader> shader) {
			shader->SetUniform<T>(name, value);
		}

		void SetValue(const T& value) {
			this->value = value;
		}
	private:
		T value;
		std::string name;

	};

	class Material {
	public:
		void SetShader(Ref<Shader> shader);

		Ref<Shader> GetShader();

		template<typename T>
		void SetProperty(const std::string& name, const T& value) {
			
			if (properties.find(name) != properties.end()) {
				auto prop = std::static_pointer_cast<MaterialProperty<T>>(properties[name]);
				if (prop) {
					prop->SetValue(value);
					return;
				}
			}
			
			properties.insert({ name, CreateRef<MaterialProperty<T>>(name, value) });
		}

		void ClearProperties();

		void BindProperties();
	private:
		Ref<Shader> shader;

		std::unordered_map<std::string, Ref<IMaterialProperty>> properties{};
	};
}