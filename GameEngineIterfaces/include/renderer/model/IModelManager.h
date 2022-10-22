#pragma once
#include <string>

class IModelManager
{
public:
	virtual ~IModelManager() = default;
	virtual bool load_model(const std::string& name, const std::string path, const std::string& shader_program_name) = 0;
};
