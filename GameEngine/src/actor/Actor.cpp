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
