#version 330 core

in vec3 inPosition;
in vec3 inNormal;

out vec3 exNormal;
uniform mat4 ModelMatrix;
uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

void main()
{
	exNormal = inNormal;
    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
} 