#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec2 texCoords;
}vs_out;

out vec3 fragPos;
out vec3 oNormal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	fragPos = vec3(model * vec4(position , 1.0f));
	oNormal = mat3(transpose(inverse(model))) * normal;
	vs_out.texCoords = TexCoords;
}