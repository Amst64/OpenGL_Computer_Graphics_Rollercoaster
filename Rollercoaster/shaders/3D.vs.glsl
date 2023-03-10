#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

// Matrices de transformations re?ues en uniform
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uNormalMatrix;

out vec3 vNormal;
out vec2 vUVCoords;
out vec3 vFragPos;

void main()
{
    // Passage en coordonn?es homog?nes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    vNormal = vec3(uNormalMatrix * vertexNormal);
    vUVCoords = aVertexTexCoords;
    vFragPos = vec3(uViewMatrix * uModelMatrix * vertexPosition);

    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertexPosition;
}