#version 330 core
#define NR_POINT_LIGHTS 2

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
};

in vec3 vNormal;
in vec2 vUVCoords;
in vec3 vFragPos;


uniform Material uMaterial;

uniform PointLight uPointLights[NR_POINT_LIGHTS];
uniform DirLight uDirLight;

out vec3 fFragColor;

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightColor = light.color;
    vec3 light_ambient = vec3(0.4, 0.4, 0.4) * lightColor;
    vec3 light_diffuse = vec3(0.5, 0.5, 0.5);
    vec3 light_specular = vec3(1, 1, 1);

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    // combine results
    vec3 ambient = light_ambient * vec3(texture(uMaterial.texture_diffuse1, vUVCoords));
    vec3 diffuse = light_diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, vUVCoords));
    vec3 specular = light_specular * spec * vec3(texture(uMaterial.texture_specular1, vUVCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightColor = light.color;
    vec3 light_ambient = vec3(0.4, 0.4, 0.4) * lightColor;
    vec3 light_diffuse = vec3(0.5, 0.5, 0.5);
    vec3 light_specular = vec3(1, 1, 1);

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    // attenuation
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    // combine results
    vec3 ambient = light_ambient * vec3(texture(uMaterial.texture_diffuse1, vUVCoords));
    vec3 diffuse = light_diffuse * diff * vec3(texture(uMaterial.texture_diffuse1, vUVCoords));
    vec3 specular = light_specular * spec * vec3(texture(uMaterial.texture_specular1, vUVCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    // properties
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(-vFragPos);

    vec3 result = CalcDirLight(uDirLight, norm, viewDir);
    //vec3 result = vec3(0, 0, 0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(uPointLights[i], norm, vFragPos, viewDir);

    fFragColor =  result;
}

