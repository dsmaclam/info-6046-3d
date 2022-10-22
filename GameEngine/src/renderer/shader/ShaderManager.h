#pragma once

#include <map>

#include <renderer/shader/IShaderManager.h>

#include <glm/glm.hpp>

class ShaderManager : public IShaderManager
{
protected:
	enum class ShaderType
	{
		vertex,
		fragment
	};

	std::map<std::string, const int> shader_programs_;


	bool compile_shader(const std::string& source, ShaderType type, int* shader_id_out);
public:
	struct ShaderVertexLayout
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	bool load_program(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path) override;
	bool get_program_id(const std::string& name, int* program_id) const override;

	void shutdown() override;
	
};

