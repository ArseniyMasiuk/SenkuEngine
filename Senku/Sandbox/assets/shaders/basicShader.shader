#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

  
out vec2 v_texCoord;

out vec3 v_Normal;
out vec3 v_FragmentPosition;
out mat3 v_TBN;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Model;

void main()
{
	v_texCoord = aTexCoord;
	v_Normal = vec3(u_Model * vec4(aNormal, 1.0));
	v_FragmentPosition = vec3(u_Model * vec4(aPos, 1.0));

	vec3 T = normalize(vec3(u_Model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(u_Model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(u_Model * vec4(aNormal,    0.0)));
	v_TBN = mat3(T, B, N);

    gl_Position = u_ViewProjMat * u_Model *vec4(aPos, 1.0);
}



#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec2 v_texCoord;


struct Material
{
	vec3 baseColor;			// albedo
	float metallic;			// metalness
	float roughness;		// roughness
	float ambientOclusion;	// AO
};


uniform int u_TextureMask; // each bit represent binded slot for texture
uniform Material u_Material;

// each bit says if texture is binded or not

uniform sampler2D u_textureAlbedo;		// 00001
uniform sampler2D u_TextureNormal;		// 00010
uniform sampler2D u_TextureRoughness;	// 00100
uniform sampler2D u_TextureMetalness;	// 01000


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//lights
struct DirLight {
	vec3 lightColor;
	vec3 direction;
};

in vec3 v_Normal;
in vec3 v_FragmentPosition;

in mat3 v_TBN;

uniform DirLight dirLight; // basically can be only one (like sun, other lightsh will be point lights or)
uniform vec3 u_CameraPosition;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// PBR// 
// what sould be set
// vec3 v_FragmentPosition;
// vec3 v_Normal;
// vec3 u_CameraPosition
// dirLight.direction // position
// dirLight.lightColor
// u_textureAlbedo or u_Material.baseColor
// float u_Material.metallic;
// float u_Material.roughness;
// float u_Material.ambientOclusion

const float PI = 3.14159265359;

vec3 fresnelSchlick(vec3 F0, vec3 V, vec3 H);
// GGX/ throwbridge-Reitz Normal distribution function
float DistributionGGX(float alpha, vec3 N, vec3 H);

// Schlick-Beckmann Geometry Shadowing Function (G1)
float GeometrySchlickGGX(float alpha, vec3 N, vec3 X);
// Smith model
float GeometrySmith(float alpha, vec3 N, vec3 V, vec3 L);
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{

	vec3 albedo;
	vec3 normal;
	float metallic;
	float roughness;
	float ao = 0.1; // ambint occlusion
	// loading values from textures or from base material 
	// depends on if certain texture was loaded or not
	// so after that we can freely use values to calculate stuff
	{
		int albedoLoaded = (u_TextureMask & 1);
		int normalLoaded = (u_TextureMask & 2);
		int roughnessLoaded = (u_TextureMask & 4);
		int metalnessLoaded = (u_TextureMask & 8);

		if (albedoLoaded == 1)
			albedo = texture(u_textureAlbedo, v_texCoord).rgb;
		else
			albedo = u_Material.baseColor;

		if (normalLoaded == 2)
		{
			normal = texture(u_TextureNormal, v_texCoord).rgb;
			normal = normal * 2.0 - 1.0;
			normal = normalize(v_TBN * normal);
		}
		else
		{
			normal = normalize(v_Normal);
		}

		if (roughnessLoaded == 4)
			roughness = texture(u_TextureRoughness, v_texCoord).r;
		else
			roughness = u_Material.roughness;

		if (metalnessLoaded == 8)
			metallic = texture(u_TextureMetalness, v_texCoord).r;
		else
			metallic = u_Material.metallic;

		// todo: add ambient occlusion texture
		ao = u_Material.ambientOclusion;

		// additional stuff
		albedo = pow(albedo, vec3(2.2));
	}


	/////////////////////////////////////////////////////////////////////////
	// main vectors
	vec3 N = normalize(normal);
	vec3 V = normalize(u_CameraPosition - v_FragmentPosition);

	// for directional light
	vec3 L = normalize(dirLight.direction - v_FragmentPosition);

	// for point lights and spot lights ?? 
	//vec3 L = normalize(dirLight.direction - v_FragmentPosition);
	vec3 H = normalize(V + L);

	float distance = length(dirLight.direction - v_FragmentPosition);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = dirLight.lightColor /** attenuation*/;

	vec3 F0 = vec3(0.04, 0.04, 0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Ks = fresnelSchlick(F0, V, H); // here can be issue since i`m not sure about H parameter
	vec3 Kd = vec3(1.0) - Ks;
	Kd *= 1.0 - metallic;

	vec3 lambert = albedo / PI;

	vec3 cookTorranceNumerator = DistributionGGX(roughness, N, H) * GeometrySmith(roughness, N, V, L) * fresnelSchlick(F0, V, H);
	float cookTorranceDenumerator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // here learnOengl adds 0.0001 to the end
	cookTorranceDenumerator = max(cookTorranceDenumerator, 0.000001);

	vec3 specular = cookTorranceNumerator / cookTorranceDenumerator;

	//vec3 BRDF = Kd * lambert + specular;

	float NdotL= max(dot(N, L),0.0); // learn openGL says dot(N, L)
	

	vec3 Lo = (Kd * albedo/PI + specular) * radiance * NdotL;

	///////////////////////////////////////////////////////////////////////


	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
} 

//////////////////////////////////////////////////////////////////////////////////
vec3 fresnelSchlick(vec3 F0, vec3 V, vec3 H)
{
	float cosTheta = max(dot(H, V), 0.0);

	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float alpha, vec3 N, vec3 H)
{
	float a = alpha * alpha;
	float a2 = a * a;
	
	float NdotH = max(dot(N, H), 0.0);

	float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;

	return a2 / max(denom, 0.000001);
}

float GeometrySchlickGGX(float alpha, vec3 N, vec3 X)
{
	float numerator = max(dot(N, X), 0.0);

	float r = alpha + 1.0;

	float k = (r*r)/ 8.0;
	float denominator = max(dot(N, X), 0.0) * (1.0 - k) + k;
	denominator = max(denominator, 0.000001);

	return numerator / denominator;
}

float GeometrySmith(float alpha, vec3 N, vec3 V, vec3 L)
{
	return GeometrySchlickGGX(alpha, N, V) * GeometrySchlickGGX(alpha, N, L);
}