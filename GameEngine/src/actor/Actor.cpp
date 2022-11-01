#include "Actor.h"

std::string Actor::get_name() const
{
	return name_;
}

void Actor::set_name(const std::string& value)
{
	name_ = value;
}

glm::vec3 Actor::get_position() const
{
	return position_;
}

void Actor::set_position(const glm::vec3& new_position)
{
	position_ = new_position;
}

std::string Actor::get_model_name() const
{
	return model_name_;
}

void Actor::set_model_name(const std::string& value)
{
	model_name_ = value;
}

FMOD::Channel* Actor::get_attached_sound()
{
	return attached_sound;
}

void Actor::set_attached_sound(FMOD::Channel* channel)
{
	attached_sound = channel;
}

glm::vec3 Actor::get_velocity() const
{
	return velocity_;
}

void Actor::set_velocity(const glm::vec3& new_velocity)
{
	velocity_ = new_velocity;
}

glm::vec3 Actor::get_color() const
{
	return color_;
}

void Actor::set_color(const glm::vec3 new_color)
{
	color_ = new_color;
}

glm::vec3 Actor::get_scale() const
{
	return scale_;
}

void Actor::set_scale(const glm::vec3 new_scale)
{
	scale_ = new_scale;
}

glm::vec3 Actor::get_rotation() const
{
	return rotation;
}

void Actor::set_rotation(const glm::vec3 new_rotation)
{
	rotation = new_rotation;
}
