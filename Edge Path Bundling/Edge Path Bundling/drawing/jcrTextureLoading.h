#pragma once
#ifndef JCRTEXTURELOADING_H
#define JCRTEXTURELOADING_H

//////////////
// INCLUDES //
//////////////

// C++ Std Library
#include<iostream>
#include<vector>

// OpenGL
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Image Loader STBI
#include "stb_image.h"

namespace jcrTexture
{
	unsigned int loadTextureFromFile(char const* path, const unsigned int& textureUnit = 0, GLint wrappingMethod = GL_CLAMP_TO_EDGE, bool gammaCorrection = false)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		int Width, Height, nrOfComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* gameData = stbi_load(path, &Width, &Height, &nrOfComponents, 0);
		GLenum format;
		if (gameData)
		{
			if (nrOfComponents == 1) format =  GL_RED;
			else if (nrOfComponents == 3) format = gammaCorrection ? GL_SRGB		: GL_RGB;
			else if (nrOfComponents == 4) format = gammaCorrection ? GL_SRGB_ALPHA	: GL_RGBA; //For png
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, gameData);
			glGenerateMipmap(GL_TEXTURE_2D);
			if (nrOfComponents == 4) // transparence requires settings to look right at the edges
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMethod);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMethod);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(gameData);
			glActiveTexture(GL_TEXTURE0);
		}
		else
		{
			std::cout << "\nTexture failed to load at path: " << path << std::endl;
			stbi_image_free(gameData);
		}
		return textureID;
	}

	unsigned int createTextureColorBuffer(const unsigned int& Width, const unsigned int& Height, GLenum format = GL_RGB, const bool isMultisample = false, const unsigned int numberOfSamples = 4)
	{

		unsigned int textureColorbufferId;
		glGenTextures(1, &textureColorbufferId);
		if (isMultisample) // multisample Buffer for AntiAliasing
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbufferId);
			GLboolean useFixedSampleLocations = true; //  use identical sample locations and the same number of samples for all texels in the image
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numberOfSamples, format, Width, Height, useFixedSampleLocations);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		else // Normal texture
		{
			glBindTexture(GL_TEXTURE_2D, textureColorbufferId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		return textureColorbufferId;

	}

	unsigned int loadCubeMapFromFile(std::vector<std::string> paths)
	{
		if (paths.size() != 6)
		{
			std::cout << "ERROR: Tried to load cube map with more or less than 6 files";
			return 0;
		}
		unsigned int textureID;
		glGenTextures(1, &textureID);
		int Width, Height, nrOfComponents;
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		int i = 0;
		for (auto it : paths)
		{
			unsigned char* gameData = stbi_load(it.c_str(), &Width, &Height, &nrOfComponents, 0);
			GLenum format;
			if (gameData)
			{
				if (nrOfComponents == 1) format = GL_RED;
				else if (nrOfComponents == 3) format = GL_RGB;
				else if (nrOfComponents == 4) format = GL_RGBA; //For png

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, gameData);
				stbi_image_free(gameData);
			}
			else
			{
				std::cout << "\nCube Map " << i << " failed to load at path : " << it << std::endl;
				stbi_image_free(gameData);
			}
			i++;
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return textureID;
	}
}
#endif