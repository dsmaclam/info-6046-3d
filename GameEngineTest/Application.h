#pragma once

//helpers and utility classes
#include "DllLoader.h"

class IGameEngine;

class Application
{
protected:
	DllLoader engine_dll_loader_;
	IGameEngine* game_engine_;

	//game engine
	bool load_engine_dll();
	bool create_engine_instance();

	//audio engine
	bool initialize_audio() const;
	bool initialize_audio_engine_sub() const;
	bool initialize_audio_channel_groups_sub() const;
	bool initialize_audio_sounds_sub() const;

	//renderer
	bool initialize_renderer() const;
	bool initialize_renderer_shaders_sub() const;
	bool initialize_renderer_models_sub() const;

public:
	Application();

	bool initialize();
	bool should_run() const;
	int run() const;
	void shutdown();
};
