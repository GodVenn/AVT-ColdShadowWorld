#version 330 core
layout(location = 0) in vec3 inPosition;

uniform mat4 ModelMatrix;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

void main()
{
    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}