#shader vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 v_FragPos; // so we can use them in the frag shader
out vec3 v_Normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    v_FragPos = vec3(u_model * vec4(a_Pos, 1.0));
    // correctly handle any non-uniform scaling of the model matrix
    v_Normal = mat3(transpose(inverse(u_model))) * a_Normal; 
    
    gl_Position = u_projection * u_view * vec4(v_FragPos, 1.0);
};

#shader fragment
#version 330 core
out vec4 FragColor;
 
// inputs from v shader
in vec3 v_Normal;  
in vec3 v_FragPos;  

uniform vec3 lightPos; 
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}