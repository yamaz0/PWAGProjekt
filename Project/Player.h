#pragma once

#include "Model.h"
#include "Movement.h"

class Player
{
private:
	Model* model;
	float movementSpeed;

	

public:

	Player(Model* _model, float _movementSpeed = SPEED)
	{
		model = _model;
		movementSpeed = _movementSpeed;
	}

	void Move(const glm::vec3 direction, float deltaTime)
	{
		glm::vec3 velocity = direction * movementSpeed * deltaTime;
		model->Move(glm::vec3(direction.x, 0, direction.z));
	}

	void Render(Shader* shader)
	{
		model->Render(shader);
	}

};