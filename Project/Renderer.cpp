#include "Renderer.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//void Renderer::Draw(const VertexArray& va, const IndexBuffer & ib, const Shader& shader) const
//{
//	shader.Bind();
//
//	va.Bind();
//	//ib.Bind();
//
//	//glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
//	glDrawArrays(GL_TRIANGLES, 0, 440);
//}
//void Renderer::Draw(const Model& model, const Shader& shader) const
//{
//	shader.Bind();
//	model.Bind();
//	if (model.GetIndiciesCount() == 0)
//	{
//		glDrawArrays(GL_TRIANGLES, 0, model.GetVertexCount());
//	}
//	else
//	{
//		glDrawElements(GL_TRIANGLES, model.GetIndiciesCount(), GL_UNSIGNED_INT, nullptr);
//	}
//}
