
#version 330 core
layout(location = 0) out vec3 color;
uniform vec3 geometry_color;
uniform sampler2DShadow shadowMap;
uniform vec3 light_position_worldspace;
uniform vec3 viewPos;
in vec3 vertex_position_worldspace;
in vec3 vertex_normal_worldspace;
in vec4 vertex_position_lightspace;
float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 normal = normalize(vertex_normal_worldspace);
	vec3 lightDir = normalize(vertex_position_worldspace - vertex_normal_worldspace);
	float bias = 0.005*tan(acos(clamp( dot( normal,lightDir ), 0,1 )));
	bias = clamp(bias, 0,0.01);
	// perform perspective divide
	vec3 projCoords = vec3(fragPosLightSpace.xy, fragPosLightSpace.z-bias)/ fragPosLightSpace.w;
		// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	float shadow = 1.0;
	for (int i=0;i<4;i++){
		
		int index = i;
		
		shadow -= 0.2*(1.0-texture( shadowMap, projCoords));
}
		
	// check whether current frag pos is in shadow
	
	

	
	return shadow;
	}
void main(){
	vec3 colorObj = geometry_color;
    // ambient
    vec3 ambient = 0.1 * colorObj;
    // diffuse
    vec3 lightDir = normalize(light_position_worldspace - vertex_position_worldspace);
    vec3 normal = normalize(vertex_normal_worldspace);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - vertex_position_worldspace);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 1.0);
    
	vec3 specular = vec3(0.3) * spec; // assuming bright white light color
		float visibility = ShadowCalculation(vertex_position_lightspace);
		//"if( texture( shadowMap, vertex_position_lightspace.xy ).z  <  vertex_position_lightspace.z){
		//"visibility = 0.1;}
		//"visibility = texture(shadowMap, vec3(vertex_position_lightspace.xy, (vertex_position_lightspace.z) / vertex_position_lightspace.w));
		//"if(visibility <0.1){visibility = 0.1;}
    color = ambient+(visibility*(diffuse+specular));
}