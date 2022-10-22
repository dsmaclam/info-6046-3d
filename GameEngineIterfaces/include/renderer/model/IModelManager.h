#pragma once
#include <string>

class IModelManager
{
public:
	struct ModelDrawData
	{
		int vao;
		int num_indices;
		int shader_program;

		ModelDrawData() : vao(0), num_indices(0), shader_program(0) {}
	};

	virtual ~IModelManager() = default;

	virtual bool initialize() = 0;

	virtual bool load_model(const std::string& name, const std::string& path, const std::string& shader_program_name) = 0;
	virtual bool get_model_draw_data(const std::string name, ModelDrawData* model_data_out) = 0;

};
