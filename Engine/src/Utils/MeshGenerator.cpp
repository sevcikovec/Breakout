#include "MeshGenerator.h"
#include <cassert>
#include "../Math/Quaternion.h"
#include "../Math/Vec2.h"
#include "../Math/Coordinates.h"
#include <algorithm>

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
		int backVerticesOffset = (resolution + 1) * 4;
		
		// add all vertices and inner and outer sides
		for (int side = 0; side < 2; side++) {
			float currentRadius = side == 0 ? innerRadius : outerRadius;
			for (int i = 0; i < resolution + 1; i++) {
				float currentAngle = angleOffset * i;

				Vec2 cartesianCoors = PolarToCartesian({ currentRadius, startAngle + currentAngle});

				Vec3 normal = { cartesianCoors.x, 0, cartesianCoors.y };
				if (side == 0) normal.Mul(-1);
				normal.Normalize();

				// add lower part
				vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
				vertices.push_back(0); // y coord
				vertices.push_back(cartesianCoors.y); // z coord
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
				
				// add upper part
				vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
				vertices.push_back(height); // y coord
				vertices.push_back(cartesianCoors.y); // z coord
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);


				// add lower part for bottom face
				vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
				vertices.push_back(0); // y coord
				vertices.push_back(cartesianCoors.y); // z coord
				vertices.push_back(0);
				vertices.push_back(-1);
				vertices.push_back(0);

				// add upper part for upper face
				
				vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
				vertices.push_back(height); // y coord
				vertices.push_back(cartesianCoors.y); // z coord
				vertices.push_back(0);
				vertices.push_back(1);
				vertices.push_back(0);

				// add indices for inner and outer sides
				if (i > 0) { 
					// inner side
					
					if (side == 0) {
						// first triangle
						indices.push_back((i) * 4 + 1);
						indices.push_back((i - 1) * 4 + 1);
						indices.push_back((i - 1) * 4);
						// second triangle
						indices.push_back(i * 4);
						indices.push_back(i * 4 + 1);
						indices.push_back((i - 1) * 4);
					}
					else {
						// first triangle
						indices.push_back((i - 1) * 4 + backVerticesOffset);
						indices.push_back((i - 1) * 4 + 1 + backVerticesOffset);
						indices.push_back((i) * 4 + 1 + backVerticesOffset);
						// second triangle
						indices.push_back((i - 1) * 4 + backVerticesOffset);
						indices.push_back(i * 4 + 1 + backVerticesOffset);
						indices.push_back(i * 4 + backVerticesOffset);
					}
				}
			}
		}
		
		// add indices top and bottom
		for (int i = 1; i < resolution + 1; i++) {
			// bottom
			
			indices.push_back((i - 1) * 4 + 2);
			indices.push_back((i - 1) * 4 + 2 + backVerticesOffset);
			indices.push_back(i * 4 + 2 + backVerticesOffset);
			
			indices.push_back(i * 4 + 2 + backVerticesOffset);
			indices.push_back(i * 4 + 2);
			indices.push_back((i - 1) * 4 + 2);
			
			// top
			indices.push_back(i * 4 + backVerticesOffset + 1 + 2);
			indices.push_back((i - 1) * 4 + backVerticesOffset + 1 + 2);
			indices.push_back((i - 1) * 4 + 1 + 2);

			indices.push_back((i - 1) * 4 + 1 + 2);
			indices.push_back(i * 4 + 1 + 2);
			indices.push_back(i * 4 + backVerticesOffset + 1 + 2);
		}

		{
			for (int leftRight = 0; leftRight < 2; leftRight++) {
				for (int side = 0; side < 2; side++) {

					int mult = leftRight == 0 ? 1 : -1;

					float currentRadius = side == 0 ? innerRadius : outerRadius;
					// add right side 
					Vec2 cartesianCoors = PolarToCartesian({ currentRadius, mult * startAngle });

					Vec3 posOnCircle = { cartesianCoors.x, 0, cartesianCoors.y };

					Vec3 normal = Vec3::Right(posOnCircle, { 0,1,0 });
					normal.Mul(-mult);
					normal.Normalize();

					// add lower part
					vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
					vertices.push_back(0); // y coord
					vertices.push_back(cartesianCoors.y); // z coord
					vertices.push_back(normal.x);
					vertices.push_back(normal.y);
					vertices.push_back(normal.z);

					// add upper part
					vertices.push_back(cartesianCoors.x - centerXOffset); // x coord
					vertices.push_back(height); // y coord
					vertices.push_back(cartesianCoors.y); // z coord
					vertices.push_back(normal.x);
					vertices.push_back(normal.y);
					vertices.push_back(normal.z);
				}			
				int currVertCount = vertices.size()/6;
				if (leftRight == 1) {
					indices.push_back(currVertCount-4);
					indices.push_back(currVertCount-2);
					indices.push_back(currVertCount-3);

					indices.push_back(currVertCount - 3);
					indices.push_back(currVertCount - 2);
					indices.push_back(currVertCount - 1);
				}
				else {
					indices.push_back(currVertCount - 4);
					indices.push_back(currVertCount - 3);
					indices.push_back(currVertCount - 2);

					indices.push_back(currVertCount - 3);
					indices.push_back(currVertCount - 1);
					indices.push_back(currVertCount - 2);
				}
			}

		}
	}

	void MeshGenerator::GenerateCircle(float radius, uint32_t resolution, std::vector<float>& vertices, std::vector<uint32_t>& indices) {
		vertices.clear();
		indices.clear();
		
		// add center vertex
		// pos
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(0);
		// normal
		vertices.push_back(0.f);
		vertices.push_back(1.f);
		vertices.push_back(0.f);
		// tex coord
		vertices.push_back(0.5f);
		vertices.push_back(0.5f);
		float currentAngle = 0;
		float angleStep = 360.f / resolution;
		for (size_t i = 0; i < resolution + 1U; i++)
		{
			currentAngle = i * angleStep;

			Vec2 cartesianCoors = PolarToCartesian({radius, currentAngle});
			// pos
			vertices.push_back(cartesianCoors.x);
			vertices.push_back(0.f);
			vertices.push_back(cartesianCoors.y);
			// normal
			vertices.push_back(0.f);
			vertices.push_back(1.f);
			vertices.push_back(0.f);

			Vec2 cartesianTexCoors = PolarToCartesian({ 0.5f, currentAngle });
			// tex coord
			vertices.push_back(cartesianTexCoors.x + 0.5f);
			vertices.push_back(cartesianTexCoors.y + 0.5f);

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

	void MeshGenerator::GenerateSphere(float radius, uint32_t resolutionHorizontal, uint32_t resolutionVertical, std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		
		vertices.clear();
		indices.clear();

		float azimuthStep = 360.f / resolutionHorizontal;
		float inclinationStep = 180.f / (resolutionVertical-1);


		
		// add top vertex
		vertices.push_back(0);
		vertices.push_back(radius);
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(1);
		vertices.push_back(0);

		// add bottom vertex
		vertices.push_back(0);
		vertices.push_back(-radius);
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(-1);
		vertices.push_back(0);

		for (size_t x = 0; x < resolutionHorizontal; x++)
		{
			float currentAzimuth = x * azimuthStep;
			for (size_t y = 1; y < resolutionVertical-1; y++) {
				float currentInclination = y * inclinationStep;
				Vec3 coords = SphericalToCartesian(radius, currentInclination, currentAzimuth);

				vertices.push_back(coords.x);
				vertices.push_back(coords.y);
				vertices.push_back(coords.z);

				auto norm = coords.Normalized();
				vertices.push_back(norm.x);
				vertices.push_back(norm.y);
				vertices.push_back(norm.z);

				// add indices
				if (x > 0) {
					if (y == 1) {
					// add top triangle
					indices.push_back(0);
					indices.push_back(2 + (resolutionVertical - 2) * x);
					indices.push_back(2 + (resolutionVertical - 2) * (x - 1));
					}
					else {
						indices.push_back(2 + (resolutionVertical - 2) * (x - 1) + (y-1) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * x + (y - 1) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * (x - 1) + (y) - 1);
						
						indices.push_back(2 + (resolutionVertical - 2) * x + (y - 1) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * x + (y) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * (x - 1) + (y)-1);
					}
				}
				else {
					if (y == 1) {
						indices.push_back(0);
						indices.push_back(2);
						indices.push_back(2 + (resolutionVertical - 2) * (resolutionHorizontal-1));
					}
					else {
						indices.push_back(2 + (resolutionVertical - 2) * (resolutionHorizontal - 1) + (y - 1) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * x + (y - 1) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * (resolutionHorizontal - 1) + (y)-1);

						indices.push_back(2 + (resolutionVertical - 2) * x + (y - 1) - 1);
						indices.push_back(2 + (resolutionVertical - 2) * x + (y)-1);
						indices.push_back(2 + (resolutionVertical - 2) * (resolutionHorizontal - 1) + (y)-1);
					}
				}
			}

			// add indices
			if (x > 0) {
				
				// add bottom triangle
				indices.push_back(1);
				indices.push_back(2 + (resolutionVertical - 2) * (x) - 1);
				indices.push_back(2 + (resolutionVertical - 2) * (x+1) -1 );
			}
			else {
				indices.push_back(1);
				indices.push_back(2 + (resolutionVertical - 2) * (resolutionHorizontal) - 1);
				indices.push_back(2 + (resolutionVertical - 2) * (x + 1) - 1);
			}
			
		}
	}

	AABB_local MeshGenerator::GetAABB(std::vector<float>& vertices) {
		AABB_local aabb;
		aabb.xMax = -FLT_MAX;
		aabb.yMax = -FLT_MAX;
		aabb.zMax = -FLT_MAX;
		aabb.xMin =  FLT_MAX;
		aabb.yMin =  FLT_MAX;
		aabb.zMin =  FLT_MAX;
		for (size_t i = 0; i < vertices.size(); i += 6)
		{
			aabb.xMin = std::min(aabb.xMin, vertices[i]);
			aabb.yMin = std::min(aabb.yMin, vertices[i+1]);
			aabb.zMin = std::min(aabb.zMin, vertices[i+2]);
			aabb.xMax = std::max(aabb.xMax, vertices[i]);
			aabb.yMax = std::max(aabb.yMax, vertices[i+1]);
			aabb.zMax = std::max(aabb.zMax, vertices[i+2]);
		}

		return aabb;
	}
}