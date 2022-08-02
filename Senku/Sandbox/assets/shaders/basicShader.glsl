#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
  
out vec2 texCoord;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_ViewProjMat * u_Model *vec4(aPos, 1.0);
	texCoord = aTexCoord;
}


#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;


struct Material
{
	vec3 baseColor;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess; //specularHighlights
	float dissolve;
};


uniform int u_TextureMask; // each bit represent binded slot for texture
uniform Material u_Material;

// each bit says if texture is binded or not

uniform sampler2D u_textureAlbedo; //    00001
uniform sampler2D u_TextureNormal; //    00010
uniform sampler2D u_TextureRoughness; // 00100
uniform sampler2D u_TextureMetalness; // 10000



void main()
{
	vec4 resultColor;

	int albedoLoaded = (u_TextureMask & 1);
	int normalLoaded = (u_TextureMask & 2);

	if(albedoLoaded == 1)
		resultColor = texture(u_textureAlbedo, texCoord);
	else
		resultColor = vec4(u_Material.baseColor, 1.0);

	
	resultColor = resultColor * vec4(1.0, 1.0,1.0, u_Material.dissolve);

    FragColor = resultColor;
} 