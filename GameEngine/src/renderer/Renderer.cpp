#include "Renderer.h"
#include "OpenGL.h"
#include "model/ModelManager.h"
#include "shader/ShaderManager.h"

void Renderer::internal_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		exit();
		return;
	}

	if(window_key_callback_)
	{
		window_key_callback_(key, scan_code, action, mods);
	}
}

void Renderer::key_callback(GLFWwindow* window, const int key, const int scan_code, const int action, const int mods)
{
	instance()->internal_key_callback(window, key, scan_code, action, mods);
}

IShaderManager* Renderer::get_shader_manager()
{
	return shader_manager_;
}

IModelManager* Renderer::get_model_manager()
{
	return model_manager_;
}

bool Renderer::initialize(const InitializationSettings& settings)
{
	if(glfwInit() != GLFW_TRUE)
	{
		return false;
	}

	window_ = glfwCreateWindow(
		settings.width,
		settings.height,
		settings.title,
		nullptr,
		nullptr
	);
	if(!window_)
	{
		return false;
	}

	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		return false;
	}

	glfwSetKeyCallback(window_, key_callback);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	shader_manager_ = new ShaderManager();
	model_manager_ = new ModelManager(shader_manager_);

	return true;
}

void Renderer::shutdown()
{
	delete model_manager_;
	model_manager_ = nullptr;

	shader_manager_->shutdown();
	delete shader_manager_;
	shader_manager_ = nullptr;

	glfwDestroyWindow(window_);
	window_ = nullptr;

	glfwTerminate();
}

void Renderer::tick()
{
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT);

	//todo: rendering code
	IModelManager::ModelDrawData draw_data;
	if(model_manager_->get_model_draw_data("cube", &draw_data))
	{
		glUseProgram(draw_data.shader_program);
		glUniform3f(glGetUniformLocation(draw_data.shader_program, "color"), 0.0f, 1.0f, 0.0f);
		glBindVertexArray(draw_data.vao);
		glDrawElements(GL_TRIANGLES, draw_data.num_indices, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

	}

	glfwSwapBuffers(window_);
}

bool Renderer::render_window_open()
{
	return !glfwWindowShouldClose(window_);
}

void Renderer::exit()
{
	glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

bool Renderer::set_window_key_callback(const window_key_callback_signature function)
{
	window_key_callback_ = function;

	return true;
}
