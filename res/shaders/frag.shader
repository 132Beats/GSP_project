
#version 330 core
layout(location = 0) out vec3 color;
uniform vec3 geometry_color;
uniform sampler2D shadowMap;
uniform vec3 light_position_worldspace;
uniform vec3 viewPos;
in vec3 vertex_position_worldspace;
in vec3 vertex_normal_worldspace;
in vec4 vertex_position_lightspace;
float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
		// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

		// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
		// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
		// check whether current frag pos is in shadow
	vec3 normal = normalize(vertex_normal_worldspace);
	vec3 lightDir = normalize(vertex_position_worldspace - vertex_normal_worldspace);
	float bias = 0.005*tan(acos(clamp( dot( normal,lightDir ), 0,1 )));
	bias = clamp(bias, 0,0.01);
	bias = 0.005;
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
shadow /= 9.0;
	if(projCoords.z > 1.0)
		shadow = 0.0;
	return shadow;
	}
void main(){
	vec3 colorObj = geometry_color;
    // ambient
    vec3 ambient = 0.01 * colorObj;
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
		float visibility = 1.0-ShadowCalculation(vertex_position_lightspace);
		//"if( texture( shadowMap, vertex_position_lightspace.xy ).z  <  vertex_position_lightspace.z){
		//"visibility = 0.1;}
		//"visibility = texture(shadowMap, vec3(vertex_position_lightspace.xy, (vertex_position_lightspace.z) / vertex_position_lightspace.w));
		//"if(visibility <0.1){visibility = 0.1;}
            color = ambient+(visibility*(diffuse+specular));
}