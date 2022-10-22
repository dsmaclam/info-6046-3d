#include "Application.h"

#include <IGameEngine.h>
#include <audio/IAudioManager.h>
#include <audio/LoadSoundModes.h>
#include <renderer/IRenderer.h>
#include <renderer/shader/IShaderManager.h>
#include <renderer/model/IModelManager.h>

void window_key_callback(const int key, int scan_code, const int action, int mods);

Application::Application():game_engine_(nullptr)
{}

bool Application::load_engine_dll()
{
	return engine_dll_loader_.load("./GameEngine.dll");
}

bool Application::create_engine_instance()
{
	const auto create_engine_instance = engine_dll_loader_.get_proc_address<IGameEngine::create_engine_func_signature>("create_instance");
	if (!create_engine_instance)
	{
		return false;
	}

	game_engine_ = create_engine_instance();
	if(!game_engine_ || !game_engine_->initialize())
	{
		return false;
	}

	return true;
}

bool Application::initialize_audio() const
{
	if(!initialize_audio_engine_sub())
	{
		return false;
	}

	if(!initialize_audio_channel_groups_sub())
	{
		return false;
	}

	if(!initialize_audio_sounds_sub())
	{
		return false;
	}

	return true;
}

bool Application::initialize_audio_engine_sub() const
{
	IAudioManager* audio_manager = game_engine_->get_audio_manager();
	if (!audio_manager || !audio_manager->initialize())
	{
		return false;
	}

	return true;
}

bool Application::initialize_audio_channel_groups_sub() const
{
	IAudioManager* audio_manager = game_engine_->get_audio_manager();

	return 	audio_manager->load_channel_group("master") &&
			audio_manager->load_channel_group("music") && 
			audio_manager->set_channel_group_parent("music", "master") &&
			audio_manager->load_channel_group("fx") && 
			audio_manager->set_channel_group_parent("fx", "master");
}

bool Application::initialize_audio_sounds_sub() const
{
	IAudioManager* audio_manager = game_engine_->get_audio_manager();
	constexpr int flags = DEFAULT;

	return audio_manager->load_sound("test", "./sounds/arrow.wav", flags);
}

bool Application::initialize_renderer() const
{
	const auto renderer = game_engine_->get_renderer();
	IRenderer::InitializationSettings renderer_settings;
	renderer_settings.title = "INFO-6046 - FMod Demo";
	if (!renderer || !renderer->initialize(renderer_settings))
	{
		return false;
	}

	if(!initialize_renderer_shaders_sub())
	{
		return false;
	}

	if(!initialize_renderer_models_sub())
	{
		return false;
	}

	renderer->set_window_key_callback(window_key_callback);

	return true;
}

bool Application::initialize_renderer_shaders_sub() const
{
	if(!game_engine_->get_renderer()->get_shader_manager()->load_program("basic", "./assets/shaders/vertex.glsl", "./assets/shaders/fragment.glsl"))
	{
		return false;
	}

	return true;
}

bool Application::initialize_renderer_models_sub() const
{

	IModelManager* model_manager = game_engine_->get_renderer()->get_model_manager();

	if(!model_manager->load_model("cube", "./assets/models/cube.ply", "basic"))
	{
		return false;
	}

	return true;
}

bool Application::initialize()
{
	//load the engine dll
	if(!load_engine_dll())
	{
		return false;
	}

	//engine instance
	if(!create_engine_instance())
	{
		return false;
	}

	//audio manager
	if(!initialize_audio())
	{
		return false;
	}

	//renderer
	if(!initialize_renderer())
	{
		return false;
	}

	

	return true;
}

bool Application::should_run() const
{
	return game_engine_->get_renderer()->render_window_open();
}

int Application::run() const
{
	while(should_run())
	{
		game_engine_->get_renderer()->tick();
		game_engine_->get_audio_manager()->tick();
	}

	return 0;
}

void Application::shutdown()
{
	game_engine_->shutdown();
	delete game_engine_;
	game_engine_ = nullptr;
}
