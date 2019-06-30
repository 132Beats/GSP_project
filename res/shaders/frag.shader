#version 330 core
layout(location = 0) out vec3 color;
uniform vec3 geometry_color;
uniform sampler2D shadowMap;
uniform vec3 light_position_worldspace;
in vec3 vertex_position_worldspace;
in vec3 vertex_normal_worldspace;
in vec4 vertex_position_lightspace;
float ShadowCalculation(vec4 fragPosLightSpace){
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	vec3 lightDir = normalize(vertex_position_worldspace - vertex_normal_worldspace);
	float bias =max(0.05 * (1.0 - dot(vertex_normal_worldspace, lightDir)), 0.005);
	float shadow = currentDepth-bias > closestDepth ? 1.0 : 0.0;
	if(projCoords.z > 1.0)
			shadow = 0.0;
	return shadow;
}

void main(){
	float visibility = 1.0-ShadowCalculation(vertex_position_lightspace);
	color = visibility*geometry_color;
    }