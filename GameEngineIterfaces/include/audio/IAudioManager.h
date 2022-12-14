#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <FMOD/fmod.hpp>

class IAudioManager
{
public:
	struct InitializationSettings
	{
		int max_channels;

		InitializationSettings(const int max_channels = 512) : max_channels(max_channels) {}
	};

	virtual ~IAudioManager() = default;
	virtual bool initialize(const InitializationSettings& settings = InitializationSettings()) = 0;
	virtual bool tick(const glm::vec3& camera_position) = 0;
	virtual void shutdown() = 0;

	//sounds
	virtual bool load_sound(const std::string& name, const std::string& path, int flags) = 0;
	virtual bool unload_sound(const std::string& name) = 0;
	virtual bool play_sound(const std::string& name) = 0;
	virtual bool play_sound(const std::string& sound_name, const std::string& channel_group_name) = 0;
	virtual bool play_sound(const std::string& sound_name, glm::vec3 position, float max_distance, FMOD::Channel** channel) = 0;

	//channel groups
	virtual bool load_channel_group(const std::string& name) = 0;
	virtual bool unload_channel_group(const std::string& name) = 0;
	virtual bool set_channel_group_parent(const std::string& child_name, const std::string& parent_name) = 0;

	virtual bool get_channel_group_volume(const std::string& name, float** volume) = 0;
	virtual bool set_channel_group_volume(const std::string& name, float volume) = 0;

	//3d sounds
	virtual bool set_listener_position(const glm::vec3 position) = 0;
	virtual bool update_3d_sound_position(FMOD::Channel* channel, const glm::vec3 position) = 0;
	virtual bool update_sound_volume(FMOD::Channel* channel, const float new_volume) = 0;
};
