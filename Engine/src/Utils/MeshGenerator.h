#pragma once
#include <vector>

namespace Engine {
	class MeshGenerator {
	public:
		static void GenerateArk(float innerRadius, float outerRadius, float sectorAngleSize, float height, uint32_t resolution, bool center, std::vector<float>& vertices, std::vector<uint32_t>& indices);

		static void GenerateCircle(float radius, uint32_t resolution, std::vector<float>& vertices, std::vector<uint32_t>& indices);
		
		static void GenerateSphere(float radius, uint32_t resolutionHorizontal, uint32_t resolutionVertical, std::vector<float>& vertices, std::vector<uint32_t>& indices);
	};
}