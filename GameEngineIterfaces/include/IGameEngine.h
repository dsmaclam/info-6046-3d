#pragma once

class IAudioManager;
class IRenderer;

class IGameEngine
{
public:
	using create_engine_func_signature = IGameEngine * (*)();

	virtual ~IGameEngine() = default;

	virtual bool initialize() = 0;
	virtual void shutdown() = 0;

	virtual IAudioManager* get_audio_manager() = 0;
	virtual IRenderer* get_renderer() = 0;
};

extern "C" __declspec(dllexport) IGameEngine* create_instance();