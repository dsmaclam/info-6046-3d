#pragma once
#include <glm/vec3.hpp>
#include <string>
#include <actor/IActor.h>
#include <FMOD/fmod.hpp>

class Actor : public IActor
{
public:
	std::string name_;
	glm::vec3 position_;
	glm::vec3 scale_ = glm::vec3(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 velocity_;
	glm::vec3 color_;
	std::string model_name_;

	FMOD::Channel* attached_sound;

	std::string get_name() const override;
	void set_name(const std::string& value) override;
	glm::vec3 get_position() const override;
	void set_position(const glm::vec3& new_position) override;
	std::string get_model_name() const override;
	void set_model_name(const std::string& value) override;
	FMOD::Channel* get_attached_sound() override;
	void set_attached_sound(FMOD::Channel* channel) override;
	glm::vec3 get_velocity() const override;
	void set_velocity(const glm::vec3& new_velocity) override;
	glm::vec3 get_color() const override;
	void set_color(const glm::vec3 new_color) override;
	glm::vec3 get_scale() const override;
	void set_scale(const glm::vec3 new_scale) override;
	glm::vec3 get_rotation() const override;
	void set_rotation(const glm::vec3 new_rotation) override;
};

