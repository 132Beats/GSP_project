#shader fragment
#version 330 core
layout(location = 0) in vec3 position;
void main(){
       	gl_Position.xyz = position;
       	gl_Position.w = 1.0;
};

#shader fragment
#version 330 core
layout(location = 0) out vec3 color;
void main(){
 	color = vec3(0.443, 0.694, 0.153);
};