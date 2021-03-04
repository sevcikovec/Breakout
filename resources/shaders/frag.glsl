#version 430 core


in vec2 textureCoord;

layout (binding = 0) uniform sampler2D object_tex;

uniform vec4 color;

layout (location = 0) out vec4 final_color;

void main() {
   final_color = color;// texture2D(object_tex,textureCoord);
}