#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 circleView;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    circleView = view * vec4(FragPos, 1.0f);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionLight {
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct PointLight 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    //sampler2D diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 circleView;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionLight directionLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform sampler2D picture;

vec3 CalculateDirectionLight(DirectionLight directionLight, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // ambient
    //vec3 ambient = directionLight.ambient * texture(material.diffuse, TexCoords).rgb;

    //// diffuse 
    //vec3 norm = normalize(Normal);

    //vec3 lightDir = normalize(-directionLight.direction);
    //    
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = directionLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    //// specular
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = directionLight.specular * spec * texture(material.specular, TexCoords).rgb;

    ////result
    //vec3 result = ambient + diffuse + specular;
    //vec3 result = CalculateLighting();

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //定向光
    vec3 result = CalculateDirectionLight(directionLight, norm, viewDir);
    //点光源
    for (int i=0; i< NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(pointLight[i], norm, FragPos, viewDir);
    //聚光
    result += CalculateSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionLight(DirectionLight directionLight, vec3 normal, vec3 viewDir)
{
    vec3 result;
    vec3 ambient = directionLight.ambient * texture(material.diffuse, TexCoords).rgb;
    // diffuse 
    vec3 lightDir = normalize(-directionLight.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionLight.specular * spec * texture(material.specular, TexCoords).rgb;

    //result
    result = ambient + diffuse + specular;
    return result;

}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 result;

    //点光源的光强衰减参数
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance +
        pointLight.quadratic * (distance * distance));

    vec3 ambient = attenuation * pointLight.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse 
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = attenuation * pointLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = attenuation * pointLight.specular * spec * texture(material.specular, TexCoords).rgb;

    //result
    result = ambient + diffuse + specular;
    return result;
}

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 result;

    vec3 lightDir = normalize(spotLight.position - fragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction));
    //聚光的边缘软化参数
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);
    // ambient（环境光）
    vec3 ambient = spotLight.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse（漫反射） 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = intensity * spotLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular（镜面反射）
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = intensity * spotLight.specular * spec * texture(material.specular, TexCoords).rgb;
    //result
    vec2 testcoord = normalize(circleView.xyz).xy;

    result = ambient + diffuse + specular + intensity * diff * texture(picture, testcoord / 0.5 + 0.5).rgb;

    return result;

}