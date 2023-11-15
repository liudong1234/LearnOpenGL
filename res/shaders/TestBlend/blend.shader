#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;
uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, v_TexCoord);
}




