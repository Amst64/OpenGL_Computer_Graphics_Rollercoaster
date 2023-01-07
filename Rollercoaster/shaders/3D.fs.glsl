#version 330 core

in vec3 vNormal;


out vec3 fFragColor;

void main() {
	fFragColor = vNormal;
}