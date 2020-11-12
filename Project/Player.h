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

	bool SphereRectCollision(Model* _model)
	{
		float distance = glm::distance(model->GetPosition(), _model->GetPosition());
		float modelsRSum = _model->GetSize() + model->GetSize();
		return distance <= modelsRSum;
	}
};