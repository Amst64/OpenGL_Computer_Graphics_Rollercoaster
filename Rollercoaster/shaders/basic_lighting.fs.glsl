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
uniform DirLight uDirLight;

uniform vec3 uViewPos;

out vec3 fFragColor;



void main()
{
    vec3 lightColor = vec3(1, 1, 1);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * vec3(texture(uMaterial.texture_diffuse1, vUVCoords));

    // diffuse 
    vec3 norm = normalize(vNormal);
    //vec3 lightDir = normalize(uPointLight.position - vFragPos);
    vec3 lightDir = normalize(-uDirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * vec3(texture(uMaterial.texture_diffuse1, vUVCoords));

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * vec3(texture(uMaterial.texture_specular1, vUVCoords));

    vec3 result = (ambient + diffuse + specular);
    fFragColor = result;
}

