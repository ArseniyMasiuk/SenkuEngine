#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

  
out vec2 texCoord;

out vec3 v_Normal;
out vec3 v_FragPos;
out mat3 v_TBN;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Model;

void main()
{
	v_Normal = aNormal;
	v_FragPos = vec3(u_Model * vec4(aPos, 1.0));

	vec3 T = normalize(vec3(u_Model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(u_Model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(u_Model * vec4(aNormal,    0.0)));
	v_TBN = mat3(T, B, N);

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//lights
struct DirLight {
	vec3 lightColor;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 v_Normal;
in vec3 v_FragPos;

in mat3 v_TBN;

uniform DirLight dirLight; // basically can be only one (like sun, other lightsh will be point lights or)
uniform vec3 u_ViewPos;


///////////////////////////////////////////////////////////////////////////////////////////////////////////


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);


void main()
{
	int albedoLoaded = (u_TextureMask & 1);
	int normalLoaded = (u_TextureMask & 2);


	vec4 resultColor;

	{


		if (albedoLoaded == 1)
			resultColor = texture(u_textureAlbedo, texCoord);
		else
			resultColor = vec4(u_Material.baseColor, 1.0);
	}


	/////////////////////////////////////////////////////////////////////////

	vec3 resultLight = vec3(0.2, 0.2, 0.2); // in case if any of lights is used, this vector will not affect to color of texture

	vec3 normal;
	if(normalLoaded == 2)
	{
		normal = texture(u_TextureNormal, texCoord).rgb;
		normal = normal * 2.0 - 1.0;
		normal = normalize(v_TBN * normal);
	}
	else
	{
		normal = normalize(v_Normal);
	}

	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	resultLight += CalcDirLight(dirLight, normal, viewDir);

	///////////////////////////////////////////////////////////////////////


	resultColor = resultColor * vec4(resultLight, 1.0);

    FragColor = resultColor;
} 


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	// combine results
	vec3 ambient = light.ambient * u_Material.diffuse;
	vec3 diffuse = light.diffuse * diff * u_Material.diffuse * light.lightColor;
	vec3 specular = light.specular * spec * u_Material.specular;

	return (ambient + diffuse + specular);
}