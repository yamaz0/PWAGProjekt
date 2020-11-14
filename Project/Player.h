#pragma once

#include "Model.h"
#include "GameObject.h"

class Player: public GameObject
{
private:
	float movementSpeed;

public:

	Player(Model* _model, float _movementSpeed = SPEED):GameObject(_model)
	{
		movementSpeed = _movementSpeed;
	}

	void Move(glm::vec3 direction)
	{
		glm::vec3 velocity = direction * movementSpeed;
		model->Move(glm::vec3(velocity.x, 0, velocity.z));
	}
};