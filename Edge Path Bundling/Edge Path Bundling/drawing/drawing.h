#pragma once

// C++ general Indices
#include<string>
#include<iostream>

//
#include<glad/glad.h>
#include<GLFW/glfw3.h> // GLFW must be included after GLAD!
#include "stb_image.h"
#include "jcrSetupGLFWandGLAD.h" //GLFWGLAD::
#include "jcrShader.h"
#include "jcrVAO.h"
#include "jcrTextureLoading.h"


void draw()
{
	int retVal;

	// Set up a rendering window using GLFW and GLAD
	GLFWwindow* window = jcrGLFWGLAD::setupWindow(retVal, 1909,823);

	// Setup Shaders
	jcrShader::Shader shaderDisplay("shaders\\flatTextureShader2D.vert", "shaders\\flatTextureShader2D.frag");
	shaderDisplay.setInt("textureUnit", 0);
	/*
	float verticesDisplay[] =
	{
		// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
		// positions          // texCoords
		-1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // left top
		-1.0f,  1.0f,  0.0f,  1.0f, 1.0f, // left bottom
		 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // right bottom 

		-1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // left top again
		 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // right bottom 
		 1.0f,  1.0f,  0.0f,  0.0f, 0.0f  // right top
	};
	*/

	float x = 1.0f;
	float y = 1.0f;
	float verticesDisplay[] = 
	{
		//Vertex Coord		// Texture Coord	
		x,  y, 0.0f,  1.0, 1.0,	// top right
		x, -y, 0.0f,  1.0, 0.0,	// bottom right
		-x,  y, 0.0f,  0.0, 1.0,	// top left 
		-x,  y, 0.0f,  0.0, 1.0,	// top left 
		x, -y, 0.0f,  1.0, 0.0,	// bottom right
		-x, -y, 0.0f,  0.0, 0.0	// bottom left
	};

	// VAO / VBO for displaying the texture
	unsigned int VAO_Display, VBO_Display;
	unsigned int dimensionOfAttributes[] = { 3,2 };
	retVal = jcrVAO::createVAOwithoutEBO(VAO_Display, VBO_Display, verticesDisplay, 6, 2, dimensionOfAttributes);

	// Load texture of map
	int texIdMap = jcrTexture::loadTextureFromFile("textures\\map.png");
	
	//RenderingLoop
	while (!glfwWindowShouldClose(window))
	{
		////////////////
		// Loop Setup //
		////////////////

		// Set color with which clear commands fills buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//clear the color buffer (using default color defined above)
		glClear(GL_COLOR_BUFFER_BIT);


		/////////////
		// Drawing //
		/////////////

		//Draw Map
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, texIdMap);
		glBindVertexArray(VAO_Display);
		shaderDisplay.use();
		shaderDisplay.setInt("textureUnit", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		////////////////////
		// Loop End Tasks //
		////////////////////

		// Swap front and back buffer (one for drawing one for displaying)
		glfwSwapBuffers(window);

		// Check for events like Keyboard input
		glfwPollEvents();
	}
}