
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
	std::vector<Mesh*> meshes;
	glm::vec3 position;

public:
	Model(
		glm::vec3 _position,
		Material* _material,
		Texture* _textureDiffuse,
		Texture* _textureSpecular,
		std::vector<Vertex> mesh
	)
	{
		position = _position;
		material = _material;
		textureDiffuse = _textureDiffuse;
		textureSpecular = _textureSpecular;

		SetMesh(mesh);
	}

	//OBJ file loaded model
	Model(
		glm::vec3 _position,
		Material* _material,
		Texture* _textureDiffuse,
		Texture* _textureSpecular,
		const char* objFile
	)
	{
		position = _position;
		material = _material;
		textureDiffuse = _textureDiffuse;
		textureSpecular = _textureSpecular;

		std::vector<Vertex> mesh = LoadOBJ(objFile);
		SetMesh(mesh);
	}

	void SetMesh(std::vector<Vertex> &mesh)
	{
		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));

		for (auto& i : meshes)
		{
			i->Move(position);
			i->SetOrigin(position);
		}
	}

	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	//Functions
	void Rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->Rotate(rotation);
	}

	void Render(Shader* shader)
	{
		//Update uniforms
		material->SetUniforms(*shader);

		//Use a program (BECAUSE SHADER CLASS LAST UNIFORM UPDATE UNUSES IT)
		shader->Bind();

		//Draw
		for (auto& i : this->meshes)
		{
			textureDiffuse->Bind(0);
			textureSpecular->Bind(1);
			i->Render(shader); //Activates shader also
		}
	}
};