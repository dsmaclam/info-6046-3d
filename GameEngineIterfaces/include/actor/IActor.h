#pragma once
#include <string>
#include <glm/vec3.hpp>
class IActor
{
public:
	virtual ~IActor() = default;

	virtual std::string get_name() const = 0;
	virtual void set_name(const std::string& value) = 0;

	virtual glm::vec3 get_position() const = 0;
	virtual void set_position(const glm::vec3& new_position) = 0;

	virtual std::string get_model_name() const = 0;
	virtual void set_model_name(const std::string& value) = 0;
};
