#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}



#shader fragment
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform Material material;
uniform Light light;

uniform float time;

void main()
{
    // ambient
    vec3 ambient = lightColor * light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * light.specular * spec * texture(material.specular, TexCoords).rgb;

    //emission
    vec2 TexCoordsTemp = TexCoords * vec2(0.8) + vec2(0.1);
    vec3 emission = time * texture(material.emission, vec2(TexCoordsTemp.x, TexCoordsTemp.y + (-cos(time) + sin(time)))).rgb;
    //vec3 emission = texture(material.emission, vec2(TexCoords.x, TexCoords.y + time/2)).rgb;
    
    //result
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}