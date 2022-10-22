#pragma once

#include <renderer/IRenderer.h>
struct GLFWwindow;
class IShaderManager;

class Renderer : public IRenderer
{
protected:
	GLFWwindow* window_;
	window_key_callback_signature window_key_callback_;
	IShaderManager* shader_manager_;
	IModelManager* model_manager_;

	Renderer() = default;

	void internal_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods);

	static void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods);
public:
	static Renderer* instance()
	{
		static Renderer instance;

		return &instance;
	}

	IShaderManager* get_shader_manager() override;
	IModelManager* get_model_manager() override;

	bool initialize(const InitializationSettings& settings) override;
	void shutdown() override;
	void tick() override;
	bool render_window_open() override;
	void exit() override;
	bool set_window_key_callback(window_key_callback_signature function) override;
	
};

