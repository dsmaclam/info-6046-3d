#pragma once

#include <map>

#include <renderer/shader/IShaderManager.h>

class ShaderManager : public IShaderManager
{
protected:
	enum class ShaderType
	{
		vertex,
		fragment
	};

	struct ShaderVertexLayout
	{
		float x, y, z;
		float r, g, b, a;
	};

	std::map<std::string, const int> shader_programs_;


	bool compile_shader(const std::string& source, ShaderType type, int* shader_id_out);
public:
	bool load_program(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path) override;
	void shutdown() override;

};

