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


void draw(Graph g)
{
	// Set up a rendering window using GLFW and GLAD
	int retVal;
	GLFWwindow* window = jcrGLFWGLAD::setupWindow(retVal, 1909,823);


	/////////////////
	// Prepare Map //
	/////////////////
	// Setup Shader
	jcrShader::Shader shaderDisplay("shaders\\map2D.vert", "shaders\\map2D.frag");
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

	// Prepare Mesh
	float x = 1.0f;
	float y = 1.0f;
	float verticesDisplay[] = 
	{
		//Vertex Coord		// Texture Coord	
		 x,  y, 1.0, 1.0,	// top right
		 x, -y, 1.0, 0.0,	// bottom right
		-x,  y, 0.0, 1.0,	// top left 
		-x,  y, 0.0, 1.0,	// top left 
		 x, -y, 1.0, 0.0,	// bottom right
		-x, -y, 0.0, 0.0	// bottom left
	};

	// VAO / VBO for displaying the texture
	unsigned int VAO_Display, VBO_Display;
	unsigned int dimOfAttributesMap[] = { 2,2 };
	retVal = jcrVAO::createVAOwithoutEBO(VAO_Display, VBO_Display, verticesDisplay, 6, 2, dimOfAttributesMap);

	// Load texture of map
	int texIdMap = jcrTexture::loadTextureFromFile("textures\\map.png");
	

	////////////////////
	// Prepare Points //
	////////////////////
	// Setup Shader
	jcrShader::Shader shaderPoints("shaders\\point2D.vert", "shaders\\point2D.frag");

	// Prepare Mesh
	float verticesPoints[] = {0,0,0.1,0.1};
	int numOfPoints = 2;

	// VAO & VBO for displaying points
	unsigned int VAO_Points, VBO_Points;
	unsigned int dimOfAttributesPoints[] = {2};
	retVal = jcrVAO::createVAOwithoutEBO(VAO_Points, VBO_Points, verticesPoints, numOfPoints, 1, dimOfAttributesPoints);

	///////////////////
	// Prepare Edges //
	///////////////////
	jcrShader::Shader shaderEdges("shaders\\edge2D.vert", "shaders\\edge2D.frag");

	// VAO & VBO for displaying points
	unsigned int VAO_Edges, VBO_Edges;
	unsigned int dimOfAttributesEdges[] = {2};
	retVal = jcrVAO::createVAOwithoutEBO(VAO_Edges, VBO_Edges, verticesPoints, numOfPoints, 1, dimOfAttributesEdges);


	/////////////
	// Drawing //
	/////////////
	// Set color with which clear commands fills buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//clear the color buffer (using default color defined above)
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw Map
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, texIdMap);
	glBindVertexArray(VAO_Display);
	shaderDisplay.use();
	shaderDisplay.setInt("textureUnit", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// Draw edges
	glBindVertexArray(VAO_Edges);
	shaderEdges.use();
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 2);
	
	// Draw Points
	glBindVertexArray(VAO_Points);
	shaderPoints.use();
	glPointSize(10.0f);
	glDrawArrays(GL_POINTS, 0, 2);




	////////////////////
	// Loop End Tasks //
	////////////////////

	// Swap front and back buffer (one for drawing one for displaying)
	glfwSwapBuffers(window);

	// Check for events like Keyboard input
	glfwPollEvents();
	
}