#include "Engine.h"

#include "audio/AudioManager.h"
#include "renderer/Renderer.h"

extern "C" __declspec(dllexport) IGameEngine* create_instance()
{
	return new Engine();
}

Engine::Engine(): audio_manager_(nullptr), renderer_(nullptr)
{
}

Engine::~Engine()
{
	delete audio_manager_;
	audio_manager_ = nullptr;
}

bool Engine::initialize()
{
	audio_manager_ = new AudioManager();
	renderer_ = Renderer::instance();

	return true;
}

void Engine::shutdown()
{
	renderer_->shutdown(); //don't delete (it is a reference)

	audio_manager_->shutdown();
	delete audio_manager_;
	audio_manager_ = nullptr;
}

IAudioManager* Engine::get_audio_manager()
{
	return audio_manager_;
}

IRenderer* Engine::get_renderer()
{
	return renderer_;
}
