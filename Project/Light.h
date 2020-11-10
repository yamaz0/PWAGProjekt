#pragma once

#include"Shader.h"
#include"glm.hpp"

class Light
{
protected:
	float intensity;
	glm::vec3 color;

public:
	Light(float _intensity, glm::vec3 _color)
	{
		this->intensity = _intensity;
		this->color = _color;
	}

	~Light()
	{

	}

	//Functions
	virtual void SetUniforms(Shader* shader) = 0;
};

class PointLight : public Light
{
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(glm::vec3 _position, float _intensity = 1.f, glm::vec3 _color = glm::vec3(1.f),
		float _constant = 1.f, float _linear = 0.045f, float _quadratic = 0.0075f)
		: Light(_intensity, _color)
	{
		position = _position;
		constant = _constant;
		linear = _linear;
		quadratic = _quadratic;
	}

	~PointLight()
	{

	}

	void SetPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	void SetUniforms(Shader* shader)
	{
		shader->SetUniform3f("pointLight.position", position.x, position.y, position.z);
		shader->SetUniform1f("pointLight.intensity", intensity);
		shader->SetUniform3f("pointLight.color", color.x, color.y, color.z);
		shader->SetUniform1f("pointLight.constant", constant);
		shader->SetUniform1f("pointLight.linear", linear);
		shader->SetUniform1f("pointLight.quadratic", quadratic);
	}
};