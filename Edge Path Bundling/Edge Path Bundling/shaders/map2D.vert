#version 330 core
layout (location = 0) in vec2 aPos;						// Define input variables that are read from vertex Attribute Pointer					// this input variable is a rgb color vector
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoordOI;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0, 1.0f);	// always write result of the calculations to gl_Position to have it rendered (mandatory) 
	TexCoordOI = aTexCoord;
};