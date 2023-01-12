#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light uLight;

uniform Material uMaterial;

in vec3 vNormal;
in vec2 vUVCoords;
in vec3 vFragPos;

uniform sampler2D uTexture;
uniform vec3 uViewPos;

out vec3 fFragColor;

void main() {
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	//ambient
	vec3 ambient = uMaterial.ambient * uLight.ambient;

	//diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLight.position - vFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uMaterial.diffuse * uLight.diffuse;

	//specular
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uMaterial.specular * spec * uLight.specular;

	fFragColor = (ambient + diffuse + specular) * vec3(texture(uTexture, vUVCoords));
}