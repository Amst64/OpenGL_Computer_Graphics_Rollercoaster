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

out vec4 fFragColor;

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light, vec4 normal, vec4 viewDir)
{
    vec4 lightColor = vec4(light.color, 1);
    vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1) * lightColor;
    vec4 light_diffuse = vec4(0.7, 0.7, 0.7, 1);
    vec4 light_specular = vec4(1, 1, 1, 1);

    vec4 lightDir = vec4(normalize(-light.direction), 0);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    // combine results
    vec4 texColor = texture(uMaterial.texture_diffuse1, vUVCoords);
    if (texColor.a < 0.1)
        discard;
    vec4 ambient = light_ambient * texColor;
    vec4 diffuse = light_diffuse * diff * texColor;
    vec4 specular = light_specular * spec * texture(uMaterial.texture_specular1, vUVCoords);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec4 CalcPointLight(PointLight light, vec4 normal, vec3 fragPos, vec4 viewDir)
{
    vec4 lightColor = vec4(light.color, 1);
    vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1) * lightColor;
    vec4 light_diffuse = vec4(0.5, 0.5, 0.5, 1);
    vec4 light_specular = vec4(1, 1, 1, 1);

    vec4 lightDir = vec4(normalize(light.position - fragPos), 0);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    // attenuation
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    // combine results
    vec4 texColor = texture(uMaterial.texture_diffuse1, vUVCoords);
    if (texColor.a < 0.1)
        discard;
    vec4 ambient = light_ambient * texColor;
    vec4 diffuse = light_diffuse * diff * texColor;
    vec4 specular = light_specular * spec * texture(uMaterial.texture_specular1, vUVCoords);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{

    // properties
    vec4 norm = vec4(normalize(vNormal), 0);
    vec4 viewDir = vec4(normalize(-vFragPos), 0);

    vec4 result = CalcDirLight(uDirLight, norm, viewDir);
    //vec3 result = vec3(0, 0, 0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(uPointLights[i], norm, vFragPos, viewDir);

    fFragColor =  result;
}

