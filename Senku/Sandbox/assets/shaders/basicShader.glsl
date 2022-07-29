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

uniform sampler2D u_textureAlbedo;

void main()
{
    FragColor = texture(u_textureAlbedo, texCoord);//vec4(texCoord, 0.0, 1.0);
} 
