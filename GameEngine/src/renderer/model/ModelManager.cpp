#include "ModelManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../OpenGL.h"

bool ModelManager::load_model(const std::string& name, const std::string& path, const std::string& shader_program_name)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	const aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];

	std::vector<ShaderManager::ShaderVertexLayout> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		ShaderManager::ShaderVertexLayout vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh data_for_gpu;
	data_for_gpu.vertices = vertices;
	data_for_gpu.indices = indices;
	shader_manager_->get_program_id(shader_program_name, &data_for_gpu.shader_program_id);

	if(!load_on_gpu(data_for_gpu))
	{
		return false;
	}

	loaded_meshes_.emplace(name, data_for_gpu);

	return true;
}

bool ModelManager::initialize()
{
	return false;
}

bool ModelManager::load_on_gpu(Mesh& mesh)
{
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);
	glGenBuffers(1, &mesh.ebo);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(ShaderManager::ShaderVertexLayout), &mesh.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int),
		&mesh.indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShaderManager::ShaderVertexLayout), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShaderManager::ShaderVertexLayout), (void*)offsetof(ShaderManager::ShaderVertexLayout, normal));

	glBindVertexArray(0);

	return true;
}

bool ModelManager::get_model_draw_data(const std::string name, ModelDrawData* model_data_out)
{
	const auto model = loaded_meshes_.find(name);
	if(model == loaded_meshes_.end())
	{
		return false;
	}

	model_data_out->vao = model->second.vao;
	model_data_out->num_indices = model->second.indices.size();
	model_data_out->shader_program = model->second.shader_program_id;

	return true;
}
