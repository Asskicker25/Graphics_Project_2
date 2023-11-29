#vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 vertexColor;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 inverseModel;

out vec3 Normal;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);
	
	vec4 worlNormal = inverseModel * vec4(normal, 1.0f);
	Normal = normalize(worlNormal.xyz);
};


#fragment

#version 330 core

out vec4 color;
in vec3 Normal;

uniform vec3 solidColor;


void main()
{
    color = vec4(solidColor.xyz, 1.0);
};