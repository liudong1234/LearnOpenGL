#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec2 v_TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * aNormal;

	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core


layout(location = 0) out vec4 color;
uniform sampler2D u_Texture;
in vec2 v_TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;		//观察位置
uniform vec3 lightPos;		//灯位置
uniform vec3 lightColor;		//灯颜色
uniform vec3 objectColor;	//物体颜色

out vec4 FragColor;

void main()
{
	//环境光
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	//漫反射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	//镜面光
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0f);
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor + FragColor;
};

