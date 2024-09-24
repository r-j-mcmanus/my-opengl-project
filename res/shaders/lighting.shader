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

uniform vec3 u_lightPos; 
uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_ambiantColor;
uniform vec3 u_viewPos;

void main()
{
    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * u_ambiantColor;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * u_lightColor; 

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    FragColor = vec4(result, 1.0);
}