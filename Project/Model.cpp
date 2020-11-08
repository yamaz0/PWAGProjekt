//#include "Model.h"
//
//
//Model::Model(const void * vertex, int vertexSize, const unsigned int * incidies, int indiciesSize, VertexBufferLayout layout)
//{
//	ib = new IndexBuffer(incidies, indiciesSize);
//	vb = new VertexBuffer(vertex, vertexSize * sizeof(float));
//	//vb = new VertexBuffer(cube, 11 * 12 * sizeof(float));
//	va = new VertexArray();
//
//	//layout.Push<float>(2);
//	va->AddBuffer(*vb, layout);
//}
//
//Model::~Model()
//{
//}
//
//void Model::Bind() const
//{
//	va->Bind();
//	ib->Bind();
//}
//
//void Model::Unbind() const
//{
//	va->Unbind();
//	ib->Unbind();
//}
