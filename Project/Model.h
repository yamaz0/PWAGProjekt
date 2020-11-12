
#pragma once

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"
#include"OBJLoader.h"

class Model
{
private:
	Material* material;
	Texture* textureDiffuse;
	Texture* textureSpecular;
	Mesh* mesh;
	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;
	float size;

public:
	Model(
		glm::vec3 _position,
		Material* _material,
		Texture* _textureDiffuse,
		Texture* _textureSpecular,
		std::vector<Vertex> mesh,
		float _size = 1.0f,
		glm::vec3 _origin = glm::vec3(0.f),
		glm::vec3 _rotation = glm::vec3(0.f),
		glm::vec3 _scale = glm::vec3(1.f))
	{
		position = _position;
		size = _size;
		origin = _origin;
		rotation = _rotation;
		scale = _scale;
		material = _material;
		textureDiffuse = _textureDiffuse;
		textureSpecular = _textureSpecular;

		SetMesh(mesh);
	}

	//OBJ file loaded model
	//Model(
	//	glm::vec3 _position,
	//	Material* _material,
	//	Texture* _textureDiffuse,
	//	Texture* _textureSpecular,
	//	const char* objFile,
	//	float _size = 1.0f,
	//	glm::vec3 _origin = glm::vec3(0.f),
	//	glm::vec3 _rotation = glm::vec3(0.f),
	//	glm::vec3 _scale = glm::vec3(1.f))
	//{
	//	position = _position;
	//	size = _size;
	//	origin = _origin;
	//	rotation = _rotation;
	//	scale = _scale;
	//	material = _material;
	//	textureDiffuse = _textureDiffuse;
	//	textureSpecular = _textureSpecular;
	//	std::vector<Vertex> mesh = LoadOBJ(objFile);
	//	SetMesh(mesh);
	//}

	void SetMesh(std::vector<Vertex> &_mesh)
	{
		mesh = new Mesh(_mesh.data(), _mesh.size(), NULL, 0);

		SetPosition(position);
		SetOrigin(position);
	}

	~Model()
	{
		delete material;
		delete textureDiffuse;
		delete textureSpecular;
		delete mesh;
	}
	
	//Modifiers
	void SetSize(float value)
	{
		size = value;
	}

	void SetPosition(const glm::vec3 _position)
	{
		position = _position;
	}

	void SetOrigin(const glm::vec3 _origin)
	{
		origin = _origin;
	}

	void SetRotation(const glm::vec3 _rotation)
	{
		rotation = _rotation;
	}

	void SetScale(const glm::vec3 setScale)
	{
		scale = scale;
	}

	//Functions
	void Move(const glm::vec3 _position)
	{
		position += _position;
	}

	void Rotate(const glm::vec3 _rotation)
	{
		rotation += _rotation;
	}

	void ScaleUp(const glm::vec3 _scale)
	{
		scale += _scale;
	}

	void Render(Shader* shader)
	{
		shader->Bind();

		material->SetUniforms(*shader);

		textureDiffuse->Bind(0);
		textureSpecular->Bind(1);

		mesh->UpdateModelMatrix(position, origin, rotation, scale);
		mesh->SetUniforms(shader);
		mesh->Render(shader); 
	}

	float GetSize() { return size; }
	glm::vec3 GetPosition() { return position; }
};