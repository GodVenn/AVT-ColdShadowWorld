#version 330 core

in vec2 exTexcoord;
out vec4 FragColor;

uniform sampler2D u_ShadowMap;

void main()
{
	float depthValue = texture(u_ShadowMap, exTexcoord).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}