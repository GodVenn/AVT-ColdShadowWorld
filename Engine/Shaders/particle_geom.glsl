#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

in VS_OUT {
    vec4 color;
} gs_in[];

out vec4 fColor;
out vec2 texcoord;

const float PI = 3.1415926;
const float size = 0.05f;
 
void build(vec4 position)
{    
	fColor = gs_in[0].color;
    if(fColor.w == 0.0)
		return;
  
  
	//right-top
	vec2 vc = position.xy + vec2(0.5, 0.5)*size;
	gl_Position = Camera.ProjectionMatrix * vec4(vc, position.zw);
	texcoord = vec2(1.0, 1.0);
	EmitVertex();
	
	//left-top
	vec2 vb = position.xy + vec2(-0.5, 0.5)*size;
	gl_Position = Camera.ProjectionMatrix * vec4(vb, position.zw);
	texcoord = vec2(0.0, 1.0);
	EmitVertex();    
	
	//right-bottom
	vec2 vd = position.xy + vec2(0.5, -0.5)*size;
	gl_Position = Camera.ProjectionMatrix * vec4(vd, position.zw);
	texcoord = vec2(1.0, 0.0);
	EmitVertex();  

  	//left-bottom 
	vec2 va = position.xy + vec2(-0.5, -0.5)*size;
	gl_Position = Camera.ProjectionMatrix * vec4(va, position.zw);
	texcoord = vec2(0.0, 0.0);
	EmitVertex();  

	EndPrimitive();  
}

void main() {    
	
    build(vec4(gl_in[0].gl_Position.xyz, 1.0));
}