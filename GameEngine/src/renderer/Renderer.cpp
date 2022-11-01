#include "Renderer.h"
#include "OpenGL.h"
#include "model/ModelManager.h"
#include "shader/ShaderManager.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../actor/Actor.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw;


Renderer::Renderer() : window_(nullptr), window_key_callback_(nullptr), shader_manager_(nullptr),
                       model_manager_(nullptr),
                       last_frame_time_(0), current_frame_time_(0),
                       frame_delta_time_(0)
{
}

void Renderer::internal_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		exit();
		return;
	}

	if (window_key_callback_)
	{
		window_key_callback_(key, scan_code, action, mods);
	}
}

void Renderer::key_callback(GLFWwindow* window, const int key, const int scan_code, const int action, const int mods)
{
	instance()->internal_key_callback(window, key, scan_code, action, mods);
}

IShaderManager* Renderer::get_shader_manager()
{
	return shader_manager_;
}

IModelManager* Renderer::get_model_manager()
{
	return model_manager_;
}

bool Renderer::initialize(const InitializationSettings& settings)
{
	if (glfwInit() != GLFW_TRUE)
	{
		return false;
	}

	window_ = glfwCreateWindow(
		settings.width,
		settings.height,
		settings.title,
		nullptr,
		nullptr
	);
	if (!window_)
	{
		return false;
	}

	glfwMakeContextCurrent(window_);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		return false;
	}

	glfwSetKeyCallback(window_, key_callback);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	shader_manager_ = new ShaderManager();
	model_manager_ = new ModelManager(shader_manager_);

	return true;
}

void Renderer::shutdown()
{
	delete model_manager_;
	model_manager_ = nullptr;

	shader_manager_->shutdown();
	delete shader_manager_;
	shader_manager_ = nullptr;

	glfwDestroyWindow(window_);
	window_ = nullptr;

	glfwTerminate();
}

float angle = 0;



void Renderer::tick(const std::vector<IActor*> actors)
{
	current_frame_time_ = glfwGetTime();
	frame_delta_time_ = last_frame_time_ - current_frame_time_;
	last_frame_time_ = current_frame_time_;

	//todo:move this code (user input) to a its own class
	if(glfwGetKey(window_, GLFW_KEY_W))
	{
		cameraPos -= 10.0f * cameraFront * frame_delta_time_;
	}

	if (glfwGetKey(window_, GLFW_KEY_S))
	{
		cameraPos += 10.0f * cameraFront * frame_delta_time_;
	}

	if (glfwGetKey(window_, GLFW_KEY_A))
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * 10.0f * frame_delta_time_;
	}

	if (glfwGetKey(window_, GLFW_KEY_D))
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * 10.0f * frame_delta_time_;
	}

	if (glfwGetKey(window_, GLFW_KEY_Q))
	{
		cameraPos -= 10.0f * cameraUp * frame_delta_time_;
	}

	if (glfwGetKey(window_, GLFW_KEY_E))
	{
		cameraPos += 10.0f * cameraUp * frame_delta_time_;
	}

	if (glfwGetKey(window_, GLFW_KEY_T))
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(++cameraYaw)) * cos(glm::radians(0.0f));
		direction.y = sin(glm::radians(0.0f));
		direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(0.0f));
		cameraFront = glm::normalize(direction);
	}

	if (glfwGetKey(window_, GLFW_KEY_R))
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(--cameraYaw)) * cos(glm::radians(0.0f));
		direction.y = sin(glm::radians(0.0f));
		direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(0.0f));
		cameraFront = glm::normalize(direction);
	}

	if(frame_delta_time_ > 0.001f)
	{
		return;
	}

	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	for ( IActor* actor : actors)
	{
		IModelManager::ModelDrawData draw_data;
		if (model_manager_->get_model_draw_data(actor->get_model_name(), &draw_data))
		{
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 150.0f);
			glUniformMatrix4fv(glGetUniformLocation(draw_data.shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			glUniformMatrix4fv(glGetUniformLocation(draw_data.shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));

			glm::mat4 model = glm::mat4(1.0f);

			//scale
			model = glm::scale(model, actor->get_scale());

			//rotation
			model = glm::rotate(model, actor->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, actor->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, actor->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

			//position
			model = glm::translate(model, actor->get_position());


			glUniformMatrix4fv(glGetUniformLocation(draw_data.shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUseProgram(draw_data.shader_program);
			const auto color = actor->get_color();
			glUniform3f(glGetUniformLocation(draw_data.shader_program, "color"), color.r, color.g, color.b);
			glBindVertexArray(draw_data.vao);
			glDrawElements(GL_TRIANGLES, draw_data.num_indices, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);

		}
	}

	glfwSwapBuffers(window_);
}

bool Renderer::render_window_open()
{
	return !glfwWindowShouldClose(window_);
}

void Renderer::exit()
{
	glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

bool Renderer::set_window_key_callback(const window_key_callback_signature function)
{
	window_key_callback_ = function;

	return true;
}

IActor* Renderer::new_actor()
{
	return new Actor();
}

glm::vec3 Renderer::get_camera_position() const
{
	return cameraPos;
}

float Renderer::get_frame_delta_time()
{
	return frame_delta_time_;
}
