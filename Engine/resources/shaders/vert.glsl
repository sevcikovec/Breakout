#version 430 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color; 

uniform mat4 modelMat;

out vec3 vertexColor; 
  
void main()
{
    vertexColor = color;
    //vertexColor = vec3(modelMat[0][0], modelMat[1][1], modelMat[2][2]);
    gl_Position =  modelMat * vec4(pos, 0.0, 1.0); 
}

/*
out vec2 textureCoord;

const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
void main()
{
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}
*/