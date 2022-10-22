#pragma once
#include <renderer/model/IModelManager.h>

class IShaderManager;
class ModelManager : public IModelManager
{
protected:
	IShaderManager* shader_manager_;
public:
	ModelManager(IShaderManager* shader_manager) : shader_manager_(shader_manager) {}
	bool load_model(const std::string& name, const std::string path, const std::string& shader_program_name) override;
};

