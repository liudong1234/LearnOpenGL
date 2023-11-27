#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

//out vec2 TexCoord;
out VS_OUT
{
	vec2 TexCoord;
}vs_out;

void main()
{
	vs_out.TexCoord = aTexCoord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
	gl_PointSize = gl_Position.z;
}