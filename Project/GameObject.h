#pragma once

#include "Model.h"
#include"Shader.h"

class GameObject
{
protected:
	Model* model;
public:
	GameObject(Model* _model)
	{
		model = _model;
	}

	~GameObject()
	{
		delete model;
	}

	bool SphereRectCollision(Model* _model)
	{
		float distance = glm::distance(model->GetPosition(), _model->GetPosition());
		float modelsRSum = _model->GetSize() + model->GetSize();
		return distance <= modelsRSum;
	}

	void Render(Shader* shader)
	{
		model->Render(shader);
	}

	Model* GetModel() { return model; }
};