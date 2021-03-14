#version 430 core

layout (location = 0) in vec2 pos; // the position variable has attribute position 0
  
void main()
{
    gl_Position = vec4(pos, 0.0, 1.0); 
}

/*
out vec2 textureCoord;

const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
void main()
{
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}
*/