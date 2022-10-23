#pragma once
#include <audio/IAudioManager.h>
#include <FMOD/fmod.hpp>
#include <map>
#include <vector>

class AudioManager : public IAudioManager
{
protected:
	FMOD::System* fmod_system_;
	std::map<std::string, FMOD::Sound*> sounds_;
	std::map<std::string, FMOD::ChannelGroup*> channel_groups_;

	std::vector<FMOD::Channel*> playing_channels_;

	static bool is_okay(const FMOD_RESULT& result, bool display_error = true);
public:

	AudioManager() :fmod_system_(nullptr) {}
	bool initialize(const InitializationSettings& settings) override;
	void shutdown() override;

	//sounds
	bool load_sound(const std::string& name, const std::string& path, int flags) override;
	bool unload_sound(const std::string& name) override;
	bool play_sound(const std::string& name) override;
	bool play_sound(const std::string& sound_name, const std::string& channel_group_name) override;
	bool tick(const glm::vec3& camera_position) override;

	//channel groups
	bool load_channel_group(const std::string& name) override;
	bool unload_channel_group(const std::string& name) override;
	bool set_channel_group_parent(const std::string& child_name, const std::string& parent_name) override;

	bool get_channel_group_volume(const std::string& name, float** volume) override;
	bool set_channel_group_volume(const std::string& name, float volume) override;

	//3d
	bool set_listener_position(const glm::vec3 position) override;
	bool play_sound(const std::string& sound_name, glm::vec3 position, float max_distance) override;
};

