#pragma once
#include <glm/vec3.hpp>
#include <string>
#include <actor/IActor.h>

class Actor : public IActor
{
public:
	std::string name_;
	glm::vec3 position_;
	glm::vec3 color_;
	std::string model_name_;

	std::string get_name() const override;
	void set_name(const std::string& value) override;
	glm::vec3 get_position() const override;
	void set_position(const glm::vec3& new_position) override;
	std::string get_model_name() const override;
	void set_model_name(const std::string& value) override;
};

