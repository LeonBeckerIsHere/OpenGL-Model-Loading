#version 330 core

struct PointLight{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoords;
in vec3 fragPos;
in vec3 oNormal;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflect1;
uniform PointLight pointLight;
uniform PointLight pointLight2;
uniform DirLight dirLight1;
uniform vec3 viewPos;
uniform samplerCube skybox;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(oNormal);
	vec3 viewDir = normalize(viewPos-fragPos);

	vec3 result = calcDirLight(dirLight1, norm, viewDir);

	vec3 incidental = normalize(fragPos-viewPos);
	vec3 reflection = reflect(incidental, normalize(oNormal));
	vec3 gloss = vec3(texture(texture_reflect1, TexCoords)) * vec3(texture(skybox, reflection));
	color = vec4(result+gloss, 1.0f);

    
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal,lightDir),0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), 32);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0f/(light.constant + light.linear * dist + light.quadratic * (dist * dist));

		//Combine results
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal,lightDir),0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), 32);

	//Combine results
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

	return (ambient + diffuse + specular);
}