//glsl version 4.5
#version 450

//shader input
layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;
layout (location = 2) flat in int textureID;
layout (location = 3) flat in int isSolidColor;
layout (location = 4) in vec3 inNormal;
layout (location = 5) in vec3 fragPos;
//output write
layout (location = 0) out vec4 outFragColor;

//texture to access
layout(set =0, binding = 0) uniform sampler2D displayTextures[256];

void main() 
{
// ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * vec3(1,1,1);
  	
    // diffuse 
    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(vec3(0.25,-1,0));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 2.f * diff * vec3(1,1,1);
    
    // specular
    /*
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(0,1,1));
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1,1,1);  
    */
        

	if(isSolidColor > 0)
	{
		outFragColor = vec4(inColor.x, inColor.y, inColor.z, 1.0);
	}
	else
	{
        vec4 result = vec4(ambient + diffuse, 1.0);
		outFragColor = result * texture(displayTextures[textureID],inUV);
	}
	
}