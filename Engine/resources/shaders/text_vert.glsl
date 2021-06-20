#version 430 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex_coord;


out VertexData
{
	vec2 tex_coord;
} outData;

uniform mat4 modelMat;

// Data of the camera
layout (std140, binding = 0) uniform CameraData
{
	mat4 viewProjection;
    vec3 eyePos;
};

//out vec3 vertexColor; 
  
const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
void main()
{
    outData.tex_coord = tex_coord;

    gl_Position =  viewProjection * modelMat * vec4(pos, 0, 1.0); 


    //gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}

/*
out vec2 textureCoord;

const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
void main()
{
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}
*/