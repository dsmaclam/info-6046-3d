#pragma once
#include <map>
#include <vector>
#include <renderer/model/IModelManager.h>
#include "../shader/ShaderManager.h"

class IShaderManager;
class ModelManager : public IModelManager
{
protected:
	struct Mesh
	{
		unsigned int vao, vbo, ebo;
		int shader_program_id;
		std::vector<ShaderManager::ShaderVertexLayout> vertices;
		std::vector<unsigned int> indices;
	};

	IShaderManager* shader_manager_;

	std::map<std::string, Mesh> loaded_meshes_;

public:
	ModelManager(IShaderManager* shader_manager) : shader_manager_(shader_manager) {}
	bool load_model(const std::string& name, const std::string& path, const std::string& shader_program_name) override;
	bool initialize() override;

	bool load_on_gpu(Mesh& mesh);
	bool get_model_draw_data(const std::string name, ModelDrawData* model_data_out) override;
};

