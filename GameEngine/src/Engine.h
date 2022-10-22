#pragma once

#include <IGameEngine.h>

class Engine : public IGameEngine
{
protected:
	IAudioManager* audio_manager_;
	IRenderer* renderer_;
public:
	Engine();
	~Engine() override;

	bool initialize() override;
	void shutdown() override;

	IAudioManager* get_audio_manager() override;
	IRenderer* get_renderer() override;
	
};
