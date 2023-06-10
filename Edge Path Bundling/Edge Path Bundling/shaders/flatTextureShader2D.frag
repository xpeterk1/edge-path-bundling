// Store a fragment shader source code as string
// Always Define Version!
#version 330 core


in vec3 FragPos;
in vec2 TexCoordOI;
out vec4 FragColor;					// Define output (mandatory) which must be a color vector (r,g,b,transparency)

uniform sampler2D textureUnit;

void main()
{
	FragColor = texture(textureUnit, TexCoordOI);
};