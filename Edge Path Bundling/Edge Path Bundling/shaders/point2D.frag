// Store a fragment shader source code as string
// Always Define Version!
#version 330 core

out vec4 FragColor;					// Define output (mandatory) which must be a color vector (r,g,b,transparency)

void main()
{
	FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
	//gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
};