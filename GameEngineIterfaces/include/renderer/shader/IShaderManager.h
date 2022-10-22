#pragma once
#include <string>

class IShaderManager
{
public:
	virtual ~IShaderManager() = default;
	virtual bool load_program(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path) = 0;
	virtual bool get_program_id(const std::string& name, int* program_id) const = 0;
	virtual void shutdown() = 0;
};