#pragma once
#ifndef JCRVAO_H
#define JCRVAO_H

#include <numeric>
#include<glad/glad.h>
#include<GLFW/glfw3.h> // GLFW must be included after GLAD!
#include<iostream>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jcrVAO
{
	int createVAOwithoutEBO(unsigned int& VAO,unsigned int& VBO, float vertices[], const unsigned int& numberOfVertices, const unsigned int& numberOfAttributes, const unsigned int dimensionOfAttributes[], const GLenum& usage = GL_STATIC_DRAW)
		//returns ID of VAO if everything worked, else returns negative error code
		//enables all attributes
	{
		if (numberOfAttributes == 0) return -1;
		int* accumulatedDimensions = new int[numberOfAttributes];
		accumulatedDimensions[0] = dimensionOfAttributes[0];
		for (int i = 1; i < numberOfAttributes; i++)
		{
			accumulatedDimensions[i] = accumulatedDimensions[i - 1] + dimensionOfAttributes[i];
		}
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * accumulatedDimensions[numberOfAttributes-1] * numberOfVertices , vertices, usage);
		unsigned int stride = accumulatedDimensions[numberOfAttributes - 1] * (sizeof(float));
		unsigned int offset = 0;
		glVertexAttribPointer(0, dimensionOfAttributes[0], GL_FLOAT, GL_FALSE, stride, (void*)(offset));
		glEnableVertexAttribArray(0);
		for (int i = 1; i < numberOfAttributes; i++)
		{
			offset = accumulatedDimensions[i - 1] * sizeof(float);
			glVertexAttribPointer(i, dimensionOfAttributes[i], GL_FLOAT, GL_FALSE, stride, (void*)(offset));
			glEnableVertexAttribArray(i);
		}
		delete[] accumulatedDimensions;
		return 0;
	}

	struct coloredVAO
	// Consists of vertices without normals / textures, color, Shader
	{
		public:
			unsigned int VAOId = 0;
			unsigned int VBOId = 0;
			unsigned int NumberOfVertices=0;
			glm::vec3 Color = glm::vec3(1,1,1);
			jcrShader::Shader Shader = jcrShader::Shader("..\\..\\shaders\\flatColorShaderNoTexNoNormals.vert", "..\\..\\shaders\\flatColorShaderNoTexNoNormals.frag");

			coloredVAO()
			{}
			
			coloredVAO(	float vertices[], 
						const unsigned int& numberOfVertices, 
						const unsigned int& numberOfAttributes, 
						const unsigned int dimensionOfAttributes[], 
						glm::vec3 inputColor = glm::vec3(1, 0, 0), 
						std::string vertexShaderPath   = "..\\..\\shaders\\flatColorShaderNoTexNoNormals.vert", 
						std::string fragmentShaderPath = "..\\..\\shaders\\flatColorShaderNoTexNoNormals.frag",
						const GLenum & usage = GL_STATIC_DRAW
					  ):
				NumberOfVertices(numberOfVertices),
				Color(inputColor),
				Shader(jcrShader::Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str()))
			{
				glGenVertexArrays(1, &VAOId);
				glGenBuffers(1, &VBOId);
				int retVal = createVAOwithoutEBO(VAOId, VBOId, vertices, numberOfVertices, numberOfAttributes, dimensionOfAttributes);
				if (retVal < 0) std::cout << "ERROR: could not create VAO STRUCT";
			}

			void Draw(glm::vec3 inputColor = glm::vec3(-1,0,0), const GLenum drawingMode = GL_FILL )
			{
				std::cout << "\n  coloredVao.Draw using VAOid " << VAOId;
				Shader.use();
				glBindVertexArray(VAOId);
				glm::vec3 drawingColor = inputColor.x == -1? Color: inputColor;
				Shader.setVec3F("color", drawingColor);
				glPolygonMode(GL_FRONT_AND_BACK, drawingMode);
				glDrawArrays(GL_TRIANGLES, 0, NumberOfVertices);
			}

			void Draw(const GLenum drawingMode)
			{
				Draw(glm::vec3(-1, 0, 0), drawingMode);
			}

			void SetModelMatrix(glm::mat4 inputMatrix)
			{
				Shader.setMat4F("modelMatrix", inputMatrix);
			}
			void SetViewMatrix(glm::mat4 inputMatrix)
			{
				Shader.setMat4F("viewMatrix", inputMatrix);
			}
			void SetProjectionMatrix(glm::mat4 inputMatrix)
			{
				Shader.setMat4F("projectionMatrix", inputMatrix);
			}

	};

}
#endif