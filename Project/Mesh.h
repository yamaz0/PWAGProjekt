#pragma once

#include<iostream>
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"Vertex.h"


class Mesh
{  
private:
	Vertex * vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	
	//glm::vec3 size;

	glm::mat4 ModelMatrix;

	void InitVAO()
	{
		//Create VAO
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		if (nrOfIndices > 0)
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
		}

		//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}


public:
	void SetUniforms(Shader* shader)
	{
		shader->SetUniformMat4f("ModelMatrix", ModelMatrix);
	}

	void UpdateModelMatrix(glm::vec3 position, glm::vec3 origin, glm::vec3 rotation, glm::vec3 scale)
	{
		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, origin);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::translate(ModelMatrix, position - origin);
		ModelMatrix = glm::scale(ModelMatrix, scale);
	}
	//glm::vec3 GetSize()
	//{
	//	return size;
	//}

	Mesh(
		Vertex* _vertexArray,
		const unsigned& _nrOfVertices,
		GLuint* _indexArray,
		const unsigned& _nrOfIndices)
	{
		nrOfVertices = _nrOfVertices;
		nrOfIndices = _nrOfIndices;

		vertexArray = new Vertex[nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			vertexArray[i] = _vertexArray[i];
		}

		indexArray = new GLuint[nrOfIndices];
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			indexArray[i] = _indexArray[i];
		}

		InitVAO();
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		if (nrOfIndices > 0)
		{
			glDeleteBuffers(1, &EBO);
		}

		delete[] vertexArray;
		delete[] indexArray;
	}

	void Render(Shader* shader)
	{		
		//Bind VAO
		glBindVertexArray(VAO);
		shader->Bind();

		//RENDER
		if (nrOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		else
		{
			glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
		}
	}

};