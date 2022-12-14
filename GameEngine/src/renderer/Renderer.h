#pragma once

#include <vector>
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

	Renderer();

	float last_frame_time_;
	float current_frame_time_;
	float frame_delta_time_;
	
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
	void tick(std::vector<IActor*> actors) override;
	bool render_window_open() override;
	void exit() override;
	bool set_window_key_callback(window_key_callback_signature function) override;
	IActor* new_actor() override;

	glm::vec3 get_camera_position() const override;

	float get_frame_delta_time() override;
};

