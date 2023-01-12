#version 330 core

in vec3 vNormal;
in vec2 vUVCoords;
in vec3 vFragPos;

uniform sampler2D uTexture;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

out vec3 fFragColor;

void main() {
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	//ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLightPos - vFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	fFragColor = (ambient + diffuse + specular) * vec3(texture(uTexture, vUVCoords));
}