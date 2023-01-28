#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in mat4 aInstanceMatrix;

// Matrices de transformations reçues en uniform
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

out vec3 vNormal;
out vec2 vUVCoords;
out vec3 vFragPos;

void main()
{
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);
    mat4 normalMatrix = transpose(inverse(aInstanceMatrix));

    vNormal = vec3(normalMatrix * vertexNormal);
    vUVCoords = aVertexTexCoords;
    vFragPos = vec3(uViewMatrix * aInstanceMatrix * vertexPosition);

    gl_Position = uProjMatrix * uViewMatrix * aInstanceMatrix * vertexPosition;
}