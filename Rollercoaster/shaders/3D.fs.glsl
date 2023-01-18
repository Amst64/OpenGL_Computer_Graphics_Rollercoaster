#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
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

uniform vec3 uViewPos;

out vec3 fFragColor;

void main() {
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	//ambient
	vec3 ambient = vec3(texture(uMaterial.diffuse, vUVCoords)) * uLight.ambient;

	//diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLight.position - vFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(uMaterial.diffuse, vUVCoords)) * uLight.diffuse;

	//specular
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = vec3(texture(uMaterial.specular, vUVCoords)) * spec * uLight.specular;

	fFragColor = vec3(texture(uMaterial.diffuse, vUVCoords));
}