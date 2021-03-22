#version 430 core


//in vec2 textureCoord;
//in vec3 vertexColor;

//layout (binding = 0) uniform sampler2D object_tex;

uniform vec3 color;

layout (location = 0) out vec4 final_color;

void main() {
   final_color = vec4(color, 1);//color;// texture2D(object_tex,textureCoord);
}
