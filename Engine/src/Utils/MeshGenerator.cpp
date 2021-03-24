#include "MeshGenerator.h"
#include <cassert>
#include "../Math/Quaternion.h"
#include "../Math/Vec2.h"
#include "../Math/Coordinates.h"

namespace Engine {
	void MeshGenerator::GenerateArk(float innerRadius, float outerRadius, float sectorAngleSize, float height, uint32_t resolution, bool center, std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		assert(resolution > 1);
		assert(innerRadius < outerRadius);

		vertices.clear();
		indices.clear();

		float startAngle = -sectorAngleSize / 2;

		float angleOffset = sectorAngleSize / resolution;
		
		float subRadius = (outerRadius - innerRadius) / 2;

		float centerXOffset = center ? (innerRadius + subRadius) * cos(ToRadians(0)) : 0;
		int backVerticesOffset = (resolution + 1) * 2;

		// add all vertices and inner and outer sides
		for (int side = 0; side < 2; side++) {
			float currentRadius = side == 0 ? innerRadius : outerRadius;
			for (int i = 0; i < resolution + 1; i++) {
				float currentAngle = angleOffset * i;

				Vec2 cartesianCoors = PolarToCartesian({ currentRadius, startAngle + currentAngle});

				// add lower part
				vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
				vertices.push_back(0); // y coord
				vertices.push_back(cartesianCoors.y); // z coord
				// add upper part
				vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
				vertices.push_back(height); // y coord
				vertices.push_back(cartesianCoors.y); // z coord
				
				// add indices for inner and outer sides
				if (i > 0) { 
					// inner side
					
					if (side == 0) {
						// first triangle
						indices.push_back((i) * 2 + 1);
						indices.push_back((i - 1) * 2 + 1);
						indices.push_back((i - 1) * 2);
						// second triangle
						indices.push_back(i * 2);
						indices.push_back(i * 2 + 1);
						indices.push_back((i - 1) * 2);
					}
					else {
						// first triangle
						indices.push_back((i - 1) * 2 + backVerticesOffset);
						indices.push_back((i - 1) * 2 + 1 + backVerticesOffset);
						indices.push_back((i) * 2 + 1 + backVerticesOffset);
						// second triangle
						indices.push_back((i - 1) * 2 + backVerticesOffset);
						indices.push_back(i * 2 + 1 + backVerticesOffset);
						indices.push_back(i * 2 + backVerticesOffset);
					}
				}
			}
		}
		
		// add indices for sides
		indices.push_back(backVerticesOffset + 1);
		indices.push_back(backVerticesOffset);
		indices.push_back(0);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(backVerticesOffset + 1);

		indices.push_back(resolution * 2);
		indices.push_back(resolution * 2 + backVerticesOffset);
		indices.push_back(resolution * 2 + backVerticesOffset + 1);
		
		indices.push_back(resolution * 2 + backVerticesOffset + 1);
		indices.push_back(resolution * 2 + 1);
		indices.push_back(resolution * 2);
		
		// add indices top and bottom
		for (int i = 1; i < resolution + 1; i++) {
			// bottom
			
			indices.push_back((i - 1) * 2);
			indices.push_back((i - 1) * 2 + backVerticesOffset);
			indices.push_back(i * 2 + backVerticesOffset);
			
			indices.push_back(i * 2 + backVerticesOffset);
			indices.push_back(i * 2);
			indices.push_back((i - 1) * 2);
			
			// top
			indices.push_back(i * 2 + backVerticesOffset + 1);
			indices.push_back((i - 1) * 2 + backVerticesOffset + 1);
			indices.push_back((i - 1) * 2 + 1);

			indices.push_back((i - 1) * 2 + 1);
			indices.push_back(i * 2 + 1);
			indices.push_back(i * 2 + backVerticesOffset + 1);
		}
	}

	void MeshGenerator::GenerateCircle(float radius, uint32_t resolution, std::vector<float>& vertices, std::vector<uint32_t>& indices) {
		vertices.clear();
		indices.clear();
		
		// add center vertex
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(0);
		float currentAngle = 0;
		float angleStep = 360.f / resolution;
		for (size_t i = 0; i < resolution + 1U; i++)
		{
			currentAngle = i * angleStep;

			Vec2 cartesianCoors = PolarToCartesian({radius, currentAngle});
			
			vertices.push_back(cartesianCoors.x);
			vertices.push_back(0);
			vertices.push_back(cartesianCoors.y);

			// add indices
			if (i > 1) {
				indices.push_back(0);
				indices.push_back(i);
				indices.push_back(i-1);
			}
		}

		// add last index
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(resolution);
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