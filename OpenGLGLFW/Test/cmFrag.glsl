#version 330 core
in vec3 TexDir;
out vec4 color;

uniform samplerCube cubemap;

void main()
{
    
    color = texture(cubemap, TexDir);
}  