#version 330 core

in vec3 exNormal;

out vec4 FragmentColor;

void main(void)
{
	vec3 color = exNormal;
	FragmentColor = vec4(color,1.0);
}