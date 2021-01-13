#pragma once
#include"GameObject.h"

class Obstacle : public GameObject
{
private:

public:
	Obstacle(Model* _model):GameObject(_model)
	{
	}

	~Obstacle()
	{
	}

	void Update()
	{
	
	}

	void OnPlayerCollision()
	{

	}
};
