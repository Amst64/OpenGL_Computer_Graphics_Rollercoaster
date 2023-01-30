#version 330 core
#define NR_POINT_LIGHTS 2

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
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
in vec3 vTangentLightPos;
in vec3 vTangentViewPos;
in vec3 vTangentFragPos;

uniform Material uMaterial;

uniform PointLight uPointLights[NR_POINT_LIGHTS];
uniform DirLight uDirLight;
uniform bool isSpecular;
uniform vec3 uViewPos;

out vec4 fFragColor;

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light)
{

    vec4 normal2 = vec4(vNormal, 0);
    vec4 lightPos = vec4(-20, 20, -40, 1);
    vec4 lightColor = vec4(light.color, 1);
    vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1) * lightColor;
    vec4 light_diffuse = vec4(0.7, 0.7, 0.7, 1);
    vec4 light_specular = vec4(1, 1, 1, 1);

    vec4 lightDir = vec4(normalize(vTangentLightPos - vTangentFragPos), 0);

    // obtain normal from normal map in range [0,1]
    vec4 normal = vec4(texture(uMaterial.texture_normal1, vUVCoords).rgb, 0);
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // diffuse shading
    float diff = max(dot(-normal, lightDir), 0.0);
    float diff2 = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec;
    float spec2;
    vec4 viewDir;
    vec4 halfwayDir;
    if (isSpecular)
    {
        viewDir = vec4(normalize(vTangentViewPos - vTangentFragPos), 0);
        halfwayDir = normalize(lightDir + viewDir);
        spec  = pow(max(dot(-normal, halfwayDir), 0.0), uMaterial.shininess);
        spec2 = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    }
    // combine results
    vec4 texColor = texture(uMaterial.texture_diffuse1, vUVCoords);
    if (texColor.a < 0.1)
        discard;
    vec4 ambient = light_ambient * texColor;
    vec4 diffuse = light_diffuse * (diff + diff2) * texColor;
    vec4 specular = light_specular * (spec + spec2) * texture(uMaterial.texture_specular1, vUVCoords);
    if (isSpecular)
    {
        return (ambient + diffuse + specular);
    }
    else
    {
        return (ambient + diffuse);
    }
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

    vec4 result = CalcDirLight(uDirLight);
    //vec3 result = vec3(0, 0, 0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(uPointLights[i], norm, vFragPos, viewDir);

    fFragColor = result;
}

