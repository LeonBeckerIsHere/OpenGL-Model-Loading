#version 330 core
in vec3 Normal;
in vec3 Position;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
	float ratio = 1.00f/1.52f;
    vec3 incidental = normalize(Position - cameraPos);
	vec3 reflection = refract(incidental, normalize(Normal), ratio);

	color = vec4(texture(skybox, reflection).rgb, 1.0f);
}