#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 shadowMVP;
out vec3 vertex_position_worldspace;
out vec3 vertex_normal_worldspace;
out vec4 vertex_position_lightspace;
void main(){
	gl_Position = MVP * vec4(position,1);\
	vertex_normal_worldspace = (MV*vec4(normal,0)).xyz;
	vertex_position_worldspace= vec3(M*vec4(position,1.0));
	vertex_position_lightspace = (shadowMVP*vec4(position,1));
}