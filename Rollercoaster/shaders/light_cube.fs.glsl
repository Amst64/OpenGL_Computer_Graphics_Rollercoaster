#version 330 core

in vec3 vNormal;
in vec2 vUVCoords;

uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
	fFragColor = vec3(1.0, 0.0, 1.0);
}