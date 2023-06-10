#pragma once
#ifndef JCRSETUPGLFWANDGLAD_H
#define JCRSETUPGLFWANDGLAD_H

// OpenGL specific Includes
#include<glad/glad.h>
#include<GLFW/glfw3.h> // GLFW must be included after GLAD!

// C++ general includes
#include<string>
#include<iostream>

namespace jcrGLFWGLAD
{
	// Callback function that is called on window resizing: adapts the viewport
	void framebuffer_size_callback(GLFWwindow* window, const int Width, const int Height)
	{
		glViewport(0, 0, Width, Height);
	}


	// Setup GLFW Window
	GLFWwindow* setupWindow(int& retVal, const unsigned int Width, const unsigned int Height, std::string windowName = "OpenGLWindow")
	{
		// Initialize glfw
		glfwInit();

		// Set major and minor version of OpenGL to be used to 3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Use OpenGL Core Profile	
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Use this line to run on MacOs
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Create Window Object
		// Args: width, height, Name, ?, ?
		GLFWwindow* renderingWindow = glfwCreateWindow(Width, Height, windowName.c_str(), NULL, NULL);

		// return -1 if window creation failed
		if (renderingWindow == NULL)
		{
			std::cout << "Failed to create GLFW Window" << std::endl;
			glfwTerminate();
			retVal = -1;
			return renderingWindow;
		}

		// Make context of "renderingWindow" the currently used window
		glfwMakeContextCurrent(renderingWindow);

		// Inittialize GLAD where glfwGetProcAddress provides the adress of function pointers of OpenGL
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			retVal = -1;
			return renderingWindow;
		}

		// Tell OpenGl the size of the rendering window /* 
		/* Args: x lower left corner, y lower left corner, width, height
		   should be smaller or equal to glfw values for "renderingWindow" */
		glViewport(0, 0, Width, Height);

		// Register callback function for Resizing of renderingWindow
		glfwSetFramebufferSizeCallback(renderingWindow, framebuffer_size_callback);
		retVal = 0;
		return renderingWindow;
	}

	// Closes Window when user defined key is pressed
	// Args: window to be closed, glfw Key code for key that closes window
	void checkIfWindowShouldClose(GLFWwindow* window, int keyCode = GLFW_KEY_ESCAPE)
	{
		// Esc Key: Close window
		// If Esc not pressed glfwGetKey returns GLFW_RELEASE
		if (glfwGetKey(window, keyCode) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}
}
#endif