#version 430 core

in VertexData
{
	vec3 normal_ws;
	vec3 position_ws;
} inData;


// Data of the camera
layout (std140, binding = 0) uniform CameraData
{
	mat4 viewProjection;
    vec3 eyePos;
};

struct Light
{
	// See the C++ code for the documentation to individual attributes
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (std140, binding = 1) uniform LightsData
{
	vec3 global_ambient_color;
	int lights_count;
	Light lights[10];
};



//in vec2 textureCoord;
//in vec3 vertexColor;

//layout (binding = 0) uniform sampler2D object_tex;

uniform vec3 color;

layout (location = 0) out vec4 final_color;

void EvaluatePhongLight(in Light light, out vec3 amb, out vec3 dif, out vec3 spe, in vec3 normal, in vec3 position, in vec3 eye, in float shininess);

void main() {
	
	vec3 amb = global_ambient_color;
	vec3 dif = vec3(0.0);
	vec3 spe = vec3(0.0);

	vec3 N = normalize(inData.normal_ws);
	vec3 Eye = normalize(eyePos - inData.position_ws);

	for (int i = 0; i < lights_count; i++)
	{
		vec3 a, d, s;
		EvaluatePhongLight(lights[i], a, d, s, N, inData.position_ws, Eye, 200.f);
		amb += a;	dif += d;	spe += s;
	}

	vec3 final_light = color * amb + color * dif + vec3(1.) * spe;

	final_color = vec4(final_light, 1);

	//final_color = vec4(inData.normal_ws, 1);
}

void EvaluatePhongLight(in Light light, out vec3 amb, out vec3 dif, out vec3 spe, in vec3 norm, in vec3 pos, in vec3 eye, in float shi)
{
	vec3 L_not_normalized = light.position.xyz - pos * light.position.w;
	vec3 L = normalize(L_not_normalized);
	vec3 H = normalize(L + eye);

	// Calculate basic Phong factors
	float Iamb = 1.0;
	float Idif = max(dot(norm, L), 0.0);
	float Ispe = (Idif > 0.0) ? pow(max(dot(norm, H), 0.0), shi) : 0.0;


	amb = Iamb * light.ambient;
	dif = Idif * light.diffuse;
	spe = Ispe * light.specular;
}