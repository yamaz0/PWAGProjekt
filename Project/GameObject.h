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

	void Render(Shader* shader)
	{
		model->Render(shader);
	}

	Model* GetModel() { return model; }
};