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
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  

in vec3 v_Normal;  
in vec3 v_FragPos;  

uniform vec3 u_lightPos; 
uniform vec3 u_lightColor;
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Light u_light;

void main()
{
    // ambient
    vec3 ambient = u_light.ambient * u_material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);

    // specular
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.diffuse * (spec * u_material.specular); 

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}