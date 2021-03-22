#include "MeshGenerator.h"
#include <cassert>
#include "../Math/Quaternion.h"

namespace Engine {
	void MeshGenerator::GenerateArk(float innerDiameter, float outerDiameter, float sectorAngleSize, float height, uint32_t resolution, bool center, std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		assert(resolution > 1);
		assert(innerDiameter < outerDiameter);

		vertices.clear();
		indices.clear();

		float startAngle = -sectorAngleSize / 2;

		float angleOffset = sectorAngleSize / resolution;
		
		float subRadius = (outerDiameter - innerDiameter) / 2;

		float centerXOffset = center ? (innerDiameter + subRadius) * cos(ToRadians(0)) : 0;
		int backVerticesOffset = (resolution + 1) * 2;

		// add all vertices and inner and outer sides
		for (int side = 0; side < 2; side++) {
			float currentDiameter = side == 0 ? innerDiameter : outerDiameter;
			for (int i = 0; i < resolution + 1; i++) {
				float currentAngle = angleOffset * i;
				float xCircle = currentDiameter * cos(ToRadians(startAngle + currentAngle));
				float yCircle = currentDiameter * sin(ToRadians(startAngle + currentAngle));

				// add lower part
				vertices.push_back(xCircle - centerXOffset); // x coord
				vertices.push_back(0); // y coord
				vertices.push_back(yCircle); // z coord
				// add upper part
				vertices.push_back(xCircle - centerXOffset); // x coord
				vertices.push_back(height); // y coord
				vertices.push_back(yCircle); // z coord
				
				// add indices for inner and outer sides
				if (i > 0) { 
					// inner side
					
					if (side == 0) {
						// first triangle
						indices.push_back((i - 1) * 2);
						indices.push_back((i - 1) * 2 + 1);
						indices.push_back((i) * 2 + 1);
						// second triangle
						indices.push_back((i - 1) * 2);
						indices.push_back(i * 2 + 1);
						indices.push_back(i * 2);
					}
					else {
						// first triangle
						indices.push_back((i) * 2 + 1 + backVerticesOffset);
						indices.push_back((i - 1) * 2 + 1 + backVerticesOffset);
						indices.push_back((i - 1) * 2 + backVerticesOffset);
						// second triangle
						indices.push_back(i * 2 + backVerticesOffset);
						indices.push_back(i * 2 + 1 + backVerticesOffset);
						indices.push_back((i - 1) * 2 + backVerticesOffset);
					}
				}
			}
		}

		// add indices for sides
		indices.push_back(0);
		indices.push_back(backVerticesOffset);
		indices.push_back(backVerticesOffset + 1);

		indices.push_back(backVerticesOffset + 1);
		indices.push_back(1);
		indices.push_back(0);

		indices.push_back(resolution * 2 + backVerticesOffset + 1);
		indices.push_back(resolution * 2 + backVerticesOffset);
		indices.push_back(resolution * 2);
		
		indices.push_back(resolution * 2);
		indices.push_back(resolution * 2 + 1);
		indices.push_back(resolution * 2 + backVerticesOffset + 1);
		
		// add indices top and bottom
		for (int i = 1; i < resolution + 1; i++) {
			// bottom
			
			indices.push_back(i * 2 + backVerticesOffset);
			indices.push_back((i - 1) * 2 + backVerticesOffset);
			indices.push_back((i - 1) * 2);
			
			indices.push_back((i - 1) * 2);
			indices.push_back(i * 2);
			indices.push_back(i * 2 + backVerticesOffset);
			
			// top
			indices.push_back((i - 1) * 2 + 1);
			indices.push_back((i - 1) * 2 + backVerticesOffset + 1);
			indices.push_back(i * 2 + backVerticesOffset + 1);

			indices.push_back(i * 2 + backVerticesOffset + 1);
			indices.push_back(i * 2 + 1);
			indices.push_back((i - 1) * 2 + 1);
		}
		
	}

	void MeshGenerator::GenerateSphere(float radius, uint32_t resolution, std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		// TODO generation of sphere
		for (size_t j = 0; j < resolution; j++) {
			for (size_t i = 0; i < resolution; i++)
			{
				
			}
		}


	}

}