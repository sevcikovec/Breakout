#pragma once
#include "../Math/Vec3.h"
#include "../Math/Vec4.h"
#include "UniformBuffer.h"
#include <algorithm>   
#include <iostream>

namespace Engine {
	const int MAX_LIGHTS = 8;

	struct LightData {
		LightData() {

		}
		LightData(Vec3 position, Vec3 ambient, Vec3 diffuse, Vec3 specular) {
			this->position = position;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
		}
	
	private:  
		Vec3 position;	float padding0 = 0;
		Vec3 ambient;	float padding1 = 0;
		Vec3 diffuse;	float padding2 = 0;
		Vec3 specular;	float padding3 = 0;
	};

	struct LightUBOData {
		Vec3 globalAmbientLight;
		int currentLights;
		LightData lights[MAX_LIGHTS];
	};

	class LightDataUBO : public AUniformBuffer {
	public:
		LightDataUBO() : AUniformBuffer(sizeof(LightUBOData))
		{
		}

		void SetData(const LightUBOData& data) {
			glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Vec3) + sizeof(int), &data);
			if (data.currentLights > 0) {
				if (data.currentLights > MAX_LIGHTS) {
					std::cout << "Trying to add: " << data.currentLights << ", max supported lights: " << MAX_LIGHTS << std::endl;
				}
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vec3) + sizeof(int), std::max(data.currentLights, MAX_LIGHTS) * sizeof(LightData), &(data.lights));

			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	protected:
		
	};
	
}