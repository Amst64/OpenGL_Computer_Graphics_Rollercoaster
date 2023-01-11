#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

// Matrices de transformations reçues en uniform
uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;

out vec3 vNormal;
out vec2 vUVCoords;


void main()
{
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    vNormal = vec3(uNormalMatrix * vertexNormal);
    vUVCoords = aVertexTexCoords;

    gl_Position = uMVPMatrix * vertexPosition;
}