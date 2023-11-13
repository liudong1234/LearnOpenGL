#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
};

uniform Material material;

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
void main()
{
	// float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
	// FragColor = texture(material.texture_diffuse1, TexCoords) + vec4(vec3(gl_FragCoord.z), 1.0);
	FragColor = texture(material.texture_diffuse1, TexCoords);
	// FragColor = ;

}
