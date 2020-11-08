#pragma once

#include<gl/glew.h>

#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc\type_ptr.hpp>

#include"Shader.h"

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	Material(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular
	)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	~Material() {}

	//Function
	void sendToShader(Shader& shader)
	{
		shader.SetUniform3f("material.ambient", ambient.x, ambient.y, ambient.z);
		shader.SetUniform3f("material.diffuse", diffuse.x, diffuse.y, diffuse.z);
		shader.SetUniform3f("material.specular", specular.x, ambient.y, ambient.z);
		shader.SetUniform1f("material.diffuseTex", 0);
		shader.SetUniform1f("material.specularTex", 1);
	}
};