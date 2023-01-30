#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

// Matrices de transformations reçues en uniform
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uViewPos;
uniform vec3 lightPos;

out vec3 vNormal;
out vec2 vUVCoords;
out vec3 vFragPos;
out vec3 vTangentLightPos;
out vec3 vTangentViewPos;
out vec3 vTangentFragPos;

void main()
{
    
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    vNormal = vec3(uNormalMatrix * vertexNormal);
    vUVCoords = aVertexTexCoords;
    vFragPos = vec3(uViewMatrix * uModelMatrix * vertexPosition);

    mat3 normalMatrix = mat3(uNormalMatrix);
    vec3 T = normalize(vec3(uModelMatrix * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(uModelMatrix * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(uModelMatrix * vec4(aVertexNormal, 0.0)));
    /*T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);*/

    mat3 TBN = transpose(mat3(T, B, N));
    vTangentLightPos = TBN * lightPos;
    vTangentViewPos = TBN * uViewPos;
    vTangentFragPos = TBN * vFragPos;

    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertexPosition;
}