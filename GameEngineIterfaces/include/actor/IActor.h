#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <FMOD/fmod.hpp>

class IActor
{
public:
	virtual ~IActor() = default;

	virtual std::string get_name() const = 0;
	virtual void set_name(const std::string& value) = 0;

	virtual glm::vec3 get_position() const = 0;
	virtual void set_position(const glm::vec3& new_position) = 0;

	virtual glm::vec3 get_velocity() const = 0;
	virtual void set_velocity(const glm::vec3& new_velocity) = 0;

	virtual std::string get_model_name() const = 0;
	virtual void set_model_name(const std::string& value) = 0;

	virtual FMOD::Channel* get_attached_sound() = 0;
	virtual void set_attached_sound(FMOD::Channel* channel) = 0;

	virtual glm::vec3 get_color() const = 0;
	virtual void set_color(const glm::vec3 new_color) = 0;

	virtual glm::vec3 get_scale() const = 0;
	virtual void set_scale(const glm::vec3 new_scale) = 0;

	virtual glm::vec3 get_rotation() const = 0;
	virtual void set_rotation(const glm::vec3 new_rotation) = 0;
};
