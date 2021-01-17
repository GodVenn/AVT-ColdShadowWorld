#version 330 core

in vec3 inPosition;

out vec3 TexCoords;

uniform mat4 ModelMatrix;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;


void main(void)
{
	TexCoords = inPosition;

	vec4 MCPosition = vec4(inPosition, 1.0);
	mat4 viewMat = mat4(mat3(Camera.ViewMatrix));
	vec4 pos = Camera.ProjectionMatrix * viewMat * ModelMatrix * MCPosition;
	gl_Position = pos.xyww;
}