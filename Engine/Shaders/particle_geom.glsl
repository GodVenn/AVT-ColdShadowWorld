#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 64) out;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

in VS_OUT {
    vec4 color;
} gs_in[];

out vec4 fColor;

const float PI = 3.1415926;
 
void build(vec4 position)
{    
    if(position.xyz != vec3(0,0,0)){
        fColor = gs_in[0].color;

	    int numberOfSides = 6;
        //Build a polygon with numberOfSide times vertices
	    for (int i = 0; i <= numberOfSides; i++) {
            float ang = PI * 2.0 / numberOfSides * i;

            vec4 offset = vec4(cos(ang) * 0.1, -sin(ang) * 0.1, 0.0, 0.0);
            gl_Position = Camera.ProjectionMatrix * (position + offset);
            EmitVertex();
        }
	    EndPrimitive();
    }
}

void main() {    
    build(vec4(gl_in[0].gl_Position.xyz, 1.0));
}