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

out vec3 fFragColor;

vec3 blinnPhong()
{
    vec3 wo = normalize(-vFragPos);
    vec3 LightDir = normalize(vec3(1, 1, 1));

    vec3 halfVector = normalize(wo + LightDir);

    vec3 LightIntensity = normalize(vec3(1, 1, 1));
    
    float ambientValue = 0.1;

    vec3 color = LightIntensity * (vec3(texture(uMaterial.texture_diffuse1, vUVCoords)) * ambientValue + vec3(texture(uMaterial.texture_diffuse1, vUVCoords)) * max(dot(LightDir, vNormal), 0.0) + vec3(texture(uMaterial.texture_specular1, vUVCoords)) * pow(max(dot(halfVector, vNormal), 0.0), uMaterial.shininess));

    return color;
}

vec3 blinnPhongPointLight()
{
    vec3 intensity = vec3(1, 0, 1);
    vec3 wo = normalize(-vFragPos);
    vec3 wi = normalize(uPointLight.position - vFragPos);
    vec3 halfVector = normalize(wo + wi);

    vec3 LightIntensity = normalize(intensity);

    vec3 color = (LightIntensity / (pow(distance(vFragPos, uPointLight.position), 2))) * (vec3(texture(uMaterial.texture_diffuse1, vUVCoords)) * max(dot(wi, vNormal), 0.0) + vec3(texture(uMaterial.texture_specular1, vUVCoords)) * pow(max(dot(halfVector, vNormal), 0.0), uMaterial.shininess));

    return color;
}

void main()
{
    fFragColor = blinnPhong() + blinnPhongPointLight();
}