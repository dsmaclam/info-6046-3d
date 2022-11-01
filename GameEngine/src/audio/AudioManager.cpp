#include "AudioManager.h"

#include <cassert>
#include <cstdio>
#include <FMOD/fmod_errors.h>

bool AudioManager::is_okay(const FMOD_RESULT& result, const bool display_error)
{
	if(result == FMOD_OK)
	{
		return true;
	}

	if(display_error)
	{
		printf("%d - %s", result, FMOD_ErrorString(result));
	}

	return false;
}

bool AudioManager::initialize(const InitializationSettings& settings)
{
	if(!is_okay(System_Create(&fmod_system_)))
	{
		return false;
	}

	if(!is_okay(fmod_system_->init(settings.max_channels, FMOD_INIT_NORMAL, nullptr)))
	{
		return false;
	}


	/*
	 * BREAK UNTIL 5:05PM
	 */

	return true;
}

void AudioManager::shutdown()
{
	//channel groups
	for(const auto& channel_group : channel_groups_)
	{
		channel_group.second->release();
	}
	channel_groups_.clear();

	//sounds
	for (const auto& sound : sounds_)
	{
		sound.second->release();
	}
	sounds_.clear();

	//system
	if(fmod_system_)
	{
		is_okay(fmod_system_->release());
	}
	fmod_system_ = nullptr;
}

bool AudioManager::load_sound(const std::string& name, const std::string& path, const int flags)
{
	assert(fmod_system_ && "no system object");
	assert(sounds_.find(name) == sounds_.end() && "sound already loaded");

	FMOD::Sound* sound;
	if(!is_okay(fmod_system_->createSound(path.c_str(), flags, nullptr, &sound)))
	{
		return false;
	}

	sounds_.emplace(name, sound);

	return true;
}

bool AudioManager::unload_sound(const std::string& name)
{
	assert(fmod_system_ && "no system object");
	assert(sounds_.find(name) != sounds_.end() && "sound not found");

	const auto sound = sounds_.find(name);
	if (sound == sounds_.end())
	{
		return false;
	}

	sound->second->release();
	sounds_.erase(sound);

	return true;
}

bool AudioManager::play_sound(const std::string& name)
{
	assert(fmod_system_ && "no system object");
	assert(sounds_.find(name) != sounds_.end() && "sound not found");

	const auto sound = sounds_.find(name);
	if(sound == sounds_.end())
	{
		return false;
	}

	FMOD::Channel* channel;
	if(!is_okay(fmod_system_->playSound(sound->second, nullptr, false, &channel)))
	{
		return false;
	}

	playing_channels_.emplace_back(channel);

	return true;
}

bool AudioManager::tick(const glm::vec3& camera_position)
{
	assert(fmod_system_ && "no system object");

	//update listener to camera position
	FMOD_VECTOR fmod_camera_position;
	fmod_camera_position.x = camera_position.x;
	fmod_camera_position.y = camera_position.y;
	fmod_camera_position.z = camera_position.z;

	is_okay(fmod_system_->set3DListenerAttributes(0, &fmod_camera_position, nullptr, nullptr, nullptr));

	return is_okay(fmod_system_->update());
}

bool AudioManager::load_channel_group(const std::string& name)
{
	assert(fmod_system_ && "no system object");
	FMOD::ChannelGroup* channel_group;

	if(!is_okay(fmod_system_->createChannelGroup(name.c_str(), &channel_group)))
	{
		return false;
	}

	channel_groups_.emplace(name, channel_group);

	return true;
}

bool AudioManager::unload_channel_group(const std::string& name)
{
	assert(fmod_system_ && "no system object");
	assert(channel_groups_.find(name) != channel_groups_.end() && "channel group not found");
	const auto channel_group = channel_groups_.find(name);
	if (channel_group == channel_groups_.end())
	{
		return false;
	}

	if(!is_okay(channel_group->second->release()))
	{
		return false;
	}

	channel_groups_.erase(channel_group);

	return true;
}

bool AudioManager::set_channel_group_parent(const std::string& child_name, const std::string& parent_name)
{
	assert(fmod_system_ && "no system object");
	assert(channel_groups_.find(child_name) != channel_groups_.end() && "child channel group not found");
	assert(channel_groups_.find(parent_name) != channel_groups_.end() && "parent channel group not found");

	const auto child_channel_group = channel_groups_.find(child_name);
	const auto parent_channel_group = channel_groups_.find(parent_name);

	if (child_channel_group == channel_groups_.end() || parent_channel_group == channel_groups_.end())
	{
		return false;
	}

	return is_okay(parent_channel_group->second->addGroup(child_channel_group->second));
}

bool AudioManager::get_channel_group_volume(const std::string& name, float** volume)
{
	assert(fmod_system_ && "no system object");
	assert(channel_groups_.find(name) != channel_groups_.end() && "channel group not found");
	const auto channel_group = channel_groups_.find(name);
	if (channel_group == channel_groups_.end())
	{
		return false;
	}

	return is_okay(channel_group->second->getVolume(*volume));
}

bool AudioManager::set_channel_group_volume(const std::string& name, const float volume)
{
	assert(fmod_system_ && "no system object");
	assert(channel_groups_.find(name) != channel_groups_.end() && "channel group not found");
	const auto channel_group = channel_groups_.find(name);
	if (channel_group == channel_groups_.end())
	{
		return false;
	}

	return is_okay(channel_group->second->setVolume(volume));
}

bool AudioManager::set_listener_position(const glm::vec3 position)
{
	FMOD_VECTOR fmod_position;
	fmod_position.x = position.x;
	fmod_position.y = position.y;
	fmod_position.z = position.z;

	return is_okay(fmod_system_->set3DListenerAttributes(0, &fmod_position, nullptr, nullptr, nullptr));
}

bool AudioManager::play_sound(const std::string& sound_name, glm::vec3 position, float max_distance, FMOD::Channel** channel)
{
	assert(fmod_system_ && "no system object");

	assert(sounds_.find(sound_name) != sounds_.end() && "sound not found");
	const auto sound = sounds_.find(sound_name);
	if (sound == sounds_.end())
	{
		return false;
	}

	if (!is_okay(fmod_system_->playSound(sound->second, nullptr, true, channel)))
	{
		return false;
	}

	FMOD_VECTOR fmod_sound_position;
	fmod_sound_position.x = position.x;
	fmod_sound_position.y = position.y;
	fmod_sound_position.z = position.z;

	if(!is_okay((*channel)->set3DAttributes(&fmod_sound_position, nullptr)))
	{
		return false;
	}

	//min distance to hear @ max volume
	//max distance where the attenuation stops
	if(!is_okay((*channel)->set3DMinMaxDistance(max_distance, 10000.0f)))
	{
		return false;
	}

	if(!is_okay((*channel)->setPaused(false)))
	{
		return false;
	}

	return true;
}

bool AudioManager::update_3d_sound_position(FMOD::Channel* channel, const glm::vec3 position)
{
	FMOD_VECTOR fmod_position;
	fmod_position.x = position.x;
	fmod_position.y = position.y;
	fmod_position.z = position.z;
	return is_okay(channel->set3DAttributes(&fmod_position, nullptr));
}

bool AudioManager::update_sound_volume(FMOD::Channel* channel, const float new_volume)
{
	return is_okay(channel->setVolume(new_volume));
}

bool AudioManager::play_sound(const std::string& sound_name, const std::string& channel_group_name)
{
	assert(fmod_system_ && "no system object");

	assert(sounds_.find(sound_name) != sounds_.end() && "sound not found");
	const auto sound = sounds_.find(sound_name);
	if (sound == sounds_.end())
	{
		return false;
	}

	assert(channel_groups_.find(channel_group_name) != channel_groups_.end() && "channel group not found");
	const auto channel_group = channel_groups_.find(channel_group_name);
	if(channel_group == channel_groups_.end())
	{
		return false;
	}

	FMOD::Channel* channel;
	if (!is_okay(fmod_system_->playSound(sound->second, channel_group->second, false, &channel)))
	{
		return false;
	}

	playing_channels_.emplace_back(channel);

	return true;
}
