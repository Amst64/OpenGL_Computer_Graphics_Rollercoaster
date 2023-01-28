#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 intensity;
};

struct PointLight {
    vec3 position;
    vec3 intensity;
};

in vec3 vNormal;
in vec2 vUVCoords;
in vec3 vFragPos;

uniform Material uMaterial;

uniform PointLight uPointLight;
//uniform DirLight uDirLight;

out vec4 fFragColor;

vec4 blinnPhong(vec3 norm)
{
    vec4 normal = vec4(norm, 0);
    vec4 wo = vec4(normalize(-vFragPos), 0);
    vec4 LightDir = vec4(normalize(vec3(1, 1, 1)), 0);

    vec4 halfVector = normalize(wo + LightDir);

    vec4 LightIntensity = vec4(normalize(vec3(1, 1, 1)),1);
    
    float ambientValue = 0.1;
    vec4 texColor = texture(uMaterial.texture_diffuse1, vUVCoords);
    if (texColor.a < 0.1)
        discard;
    vec4 color = LightIntensity * (vec4(texture(uMaterial.texture_diffuse1, vUVCoords)) * ambientValue + vec4(texture(uMaterial.texture_diffuse1, vUVCoords)) * max(dot(LightDir, normal), 0.0) + vec4(texture(uMaterial.texture_specular1, vUVCoords)) * pow(max(dot(halfVector, normal), 0.0), uMaterial.shininess));

    return color;
}

vec4 blinnPhongPointLight(vec3 norm)
{
    vec4 normal = vec4(norm, 0);
    vec3 intensity = vec3(1, 0, 1);
    vec4 wo = vec4(normalize(-vFragPos), 0);
    vec4 wi = vec4(normalize(uPointLight.position - vFragPos), 0);
    vec4 halfVector = vec4(normalize(wo + wi));

    vec4 LightIntensity = vec4(normalize(intensity), 0);
    vec4 texColor = texture(uMaterial.texture_diffuse1, vUVCoords);
    if (texColor.a < 0.1)
        discard;
    vec4 color = (LightIntensity / (pow(distance(vFragPos, uPointLight.position), 2))) * (vec4(texture(uMaterial.texture_diffuse1, vUVCoords)) * max(dot(wi, normal), 0.0) + vec4(texture(uMaterial.texture_specular1, vUVCoords)) * pow(max(dot(halfVector, normal), 0.0), uMaterial.shininess));

    return color;
}

void main()
{
    fFragColor = blinnPhong(vNormal) + blinnPhongPointLight(vNormal);
}