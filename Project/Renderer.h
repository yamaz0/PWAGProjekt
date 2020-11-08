#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Clear() const;
	//void Draw(const Model& model, const Shader& shader) const;
	//void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

