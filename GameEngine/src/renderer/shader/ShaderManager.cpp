#include "ShaderManager.h"

#include <fstream>
#include <sstream>

#include "../OpenGL.h"

bool ShaderManager::compile_shader(const std::string& source, const ShaderType type, int* shader_id_out)
{
    int id = 0;
    switch (type)
    {
    case ShaderType::vertex:
        id = glCreateShader(GL_VERTEX_SHADER);
        break;

    case ShaderType::fragment:
        id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }

    if(id == 0)
    {
        return false;
    }

    const auto c_source = source.c_str();
    glShaderSource(id, 1, &c_source, nullptr);
    glCompileShader(id);

    int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);

        return false;
    }

    *shader_id_out = id;

    return true;
}

bool ShaderManager::load_program(const std::string& name, const std::string& vertex_shader_path,
    const std::string& fragment_shader_path)
{
    std::string vertex_source;
    std::string fragment_source;

    try
    {
	    // open files
        std::ifstream vertex_file;
        vertex_file.open(vertex_shader_path);

        std::ifstream fragment_file;
    	fragment_file.open(fragment_shader_path);

        // read contents into streams
        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_file.rdbuf();
        fragment_shader_stream << fragment_file.rdbuf();

        // close file handles
        vertex_file.close();
        fragment_file.close();

        // convert stream into string
        vertex_source = vertex_shader_stream.str();
        fragment_source = fragment_shader_stream.str();
    }
    catch (const std::ifstream::failure&)
    {
        //todo: handle io error
        return false;
    }

    //compile vertex shader
    int vertex_shader_id;
    if (!compile_shader(vertex_source,ShaderType::vertex, &vertex_shader_id))
    {
        return false;
    }

    //compile fragment shader
    int fragment_shader_id;
    //create shaders on gpu
    if (!compile_shader(fragment_source,ShaderType::fragment, &fragment_shader_id))
    {
        return false;
    }

    //link the shaders together (shader program)
    int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    int success;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(shader_program_id, 512, nullptr, log);

        //todo: do something with error message

        //cleanup
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return false;
    }

    //add to shader programs map
    shader_programs_.emplace(name, shader_program_id);

    //cleanup
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return true;
}

bool ShaderManager::get_program_id(const std::string& name, int* program_id) const
{
    const auto program = shader_programs_.find(name);
    if(program == shader_programs_.end())
    {
        return false;
    }

    *program_id = program->second;

    return true;
}

void ShaderManager::shutdown()
{
    for(const auto& shader_program : shader_programs_)
    {
        glDeleteShader(shader_program.second);
    }
    shader_programs_.clear();
};
