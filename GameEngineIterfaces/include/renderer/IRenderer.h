#pragma once
#include "actor/IActor.h"
#include <glm/vec3.hpp>

class IShaderManager;
class IModelManager;

class IRenderer
{
public:
	struct InitializationSettings
	{
		int width;
		int height;
		const char* title;

		InitializationSettings(): width(800), height(600), title("renderer demo - daniel maclam"){}
	};

	virtual ~IRenderer() = default;

	virtual IShaderManager* get_shader_manager() = 0;
	virtual IModelManager* get_model_manager() = 0;

	virtual bool initialize(const InitializationSettings& settings = InitializationSettings()) = 0;
	virtual void shutdown() = 0;

	virtual void exit() = 0;
	virtual void tick(std::vector<IActor*> actors) = 0;
	virtual float get_frame_delta_time() = 0;
	virtual bool render_window_open() = 0;

	using window_key_callback_signature = void(*)(int, int, int, int);
	virtual bool set_window_key_callback(window_key_callback_signature function) = 0;

	virtual IActor* new_actor() = 0;

	virtual glm::vec3 get_camera_position() const = 0;

};
