
#pragma once

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"
#include"OBJLoader.h"

class Model
{
private:
	Material * material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

	void updateUniforms()
	{

	}

public:
	Model(
		glm::vec3 _position,
		Material* _material,
		Texture* _orTexDif,
		Texture* _orTexSpec,
		std::vector<Vertex> mesh
	)
	{
		position = _position;
		material = _material;
		overrideTextureDiffuse = _orTexDif;
		overrideTextureSpecular = _orTexSpec;

		SetMesh(mesh);
	}

	//OBJ file loaded model
	Model(
		glm::vec3 _position,
		Material* _material,
		Texture* _orTexDif,
		Texture* _orTexSpec,
		const char* objFile
	)
	{
		position = _position;
		material = _material;
		overrideTextureDiffuse = _orTexDif;
		overrideTextureSpecular = _orTexSpec;

		std::vector<Vertex> mesh = loadOBJ(objFile);
		SetMesh(mesh);
	}

	void SetMesh(std::vector<Vertex> &mesh)
	{
		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	//Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}

	void update()
	{

	}

	void render(Shader* shader)
	{
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Use a program (BECAUSE SHADER CLASS LAST UNIFORM UPDATE UNUSES IT)
		shader->Bind();

		//Draw
		for (auto& i : this->meshes)
		{
			overrideTextureDiffuse->Bind(0);
			overrideTextureSpecular->Bind(1);
			i->render(shader); //Activates shader also
		}
	}
};