#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 texCoords;
}gs_in[];

out	vec2 TexCoords;

uniform float modifier;
uniform float time;
uniform bool flag;

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a,b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 10.0f;
	vec3 direction = normal * ((sin(time*modifier)+ 1.0) / 2.0f) * magnitude;

	if(flag)
		return position + vec4(direction, 0.0f);
	else
		return position;
}

void main() {    
    vec3 normal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	TexCoords = gs_in[0].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	TexCoords = gs_in[1].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	TexCoords = gs_in[2].texCoords;
	EmitVertex();
	EndPrimitive();
}  


/*void build_house(vec4 position)
{    
	fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left   
	EmitVertex();   
	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
	EmitVertex();
	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
	EmitVertex();
	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
	EmitVertex();
	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
	EmitVertex();
	EndPrimitive();  
}*/