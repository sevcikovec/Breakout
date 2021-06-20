#version 430 core

in VertexData
{
	vec2 tex_coord;
} inData;



layout (binding = 0) uniform sampler2D font_tex;

uniform vec3 color;

layout (location = 0) out vec4 final_color;


void main() {
	
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(font_tex, inData.tex_coord).r);
    final_color = vec4(color, 1.0) * sampled;
    //final_color = vec4(inData.tex_coord, 1.0, 1.0);
	//final_color = vec4(color,1.0);
}
