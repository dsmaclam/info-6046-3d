#include "Application.h"

#include <IGameEngine.h>
#include <vector>
#include <audio/IAudioManager.h>
#include <audio/LoadSoundModes.h>
#include <glm/geometric.hpp>
#include <renderer/IRenderer.h>
#include <renderer/shader/IShaderManager.h>
#include <renderer/model/IModelManager.h>

#include "actor/IActor.h"

void window_key_callback(const int key, int scan_code, const int action, int mods);

Application::Application() :game_engine_(nullptr)
{
	
}

bool Application::load_engine_dll()
{
	return engine_dll_loader_.load("./GameEngine.dll");
}

bool Application::create_engine_instance()
{
	const auto create_engine_instance = engine_dll_loader_.get_proc_address<IGameEngine::create_engine_func_signature>("create_instance");
	if (!create_engine_instance)
	{
		return false;
	}

	game_engine_ = create_engine_instance();
	if (!game_engine_ || !game_engine_->initialize())
	{
		return false;
	}

	return true;
}

bool Application::initialize_audio() const
{
	if (!initialize_audio_engine_sub())
	{
		return false;
	}

	if (!initialize_audio_channel_groups_sub())
	{
		return false;
	}

	if (!initialize_audio_sounds_sub())
	{
		return false;
	}

	return true;
}

bool Application::initialize_audio_engine_sub() const
{
	IAudioManager* audio_manager = game_engine_->get_audio_manager();
	if (!audio_manager || !audio_manager->initialize())
	{
		return false;
	}

	return true;
}

bool Application::initialize_audio_channel_groups_sub() const
{
	IAudioManager* audio_manager = game_engine_->get_audio_manager();

	return 	audio_manager->load_channel_group("master") &&
		audio_manager->load_channel_group("music") &&
		audio_manager->set_channel_group_parent("music", "master") &&
		audio_manager->load_channel_group("fx") &&
		audio_manager->set_channel_group_parent("fx", "master");
}

bool Application::initialize_audio_sounds_sub() const
{
	IAudioManager* audio_manager = game_engine_->get_audio_manager();
	constexpr int flags = DEFAULT | _3D | LOOP_NORMAL;

	const bool result =
		audio_manager->load_sound("arrow", "./assets/sounds/arrow.wav", flags) &&
		audio_manager->load_sound("engine", "./assets/sounds/engine.mp3", flags) &&
		audio_manager->load_sound("impact", "./assets/sounds/impact.mp3", DEFAULT | _3D) &&
		audio_manager->load_sound("metal_impact", "./assets/sounds/metal_impact.wav", DEFAULT | _3D) &&
		audio_manager->load_sound("jungle", "./assets/sounds/jungle.mp3", flags);

	return result;
}

bool Application::initialize_renderer() const
{
	const auto renderer = game_engine_->get_renderer();
	IRenderer::InitializationSettings renderer_settings;
	renderer_settings.title = "INFO-6046 - FMod Demo";
	if (!renderer || !renderer->initialize(renderer_settings))
	{
		return false;
	}

	if (!initialize_renderer_shaders_sub())
	{
		return false;
	}

	if (!initialize_renderer_models_sub())
	{
		return false;
	}

	renderer->set_window_key_callback(window_key_callback);

	return true;
}

bool Application::initialize_renderer_shaders_sub() const
{
	if (!game_engine_->get_renderer()->get_shader_manager()->load_program("basic", "./assets/shaders/vertex.glsl", "./assets/shaders/fragment.glsl"))
	{
		return false;
	}

	return true;
}

bool Application::initialize_renderer_models_sub() const
{

	IModelManager* model_manager = game_engine_->get_renderer()->get_model_manager();

	if (!model_manager->load_model("sphere", "./assets/models/sphere.ply", "basic"))
	{
		return false;
	}

	if (!model_manager->load_model("cube", "./assets/models/cube.ply", "basic"))
	{
		return false;
	}

	if (!model_manager->load_model("plane", "./assets/models/plane.ply", "basic"))
	{
		return false;
	}

	return true;
}

bool Application::initialize()
{
	//load the engine dll
	if (!load_engine_dll())
	{
		return false;
	}

	//engine instance
	if (!create_engine_instance())
	{
		return false;
	}

	//audio manager
	if (!initialize_audio())
	{
		return false;
	}

	//renderer
	if (!initialize_renderer())
	{
		return false;
	}

	return true;
}

bool Application::should_run() const
{
	return game_engine_->get_renderer()->render_window_open();
}

int Application::run() const
{
	std::vector<IActor*> actors;
	{
		const auto actor = game_engine_->get_renderer()->new_actor();
		actor->set_model_name("sphere");
		actor->set_color(glm::vec3(1.0f, 0.0f, 1.0f));
		actor->set_position(glm::vec3(0.0f,0.0f,15.0f));
		actor->set_velocity(glm::vec3(0.0f, 0.0f, -2.5f));

		//we need to attach the sound to the object
		FMOD::Channel* channel;
		game_engine_->get_audio_manager()->play_sound("engine", glm::vec3(), 0.1f, &channel);
		actor->set_attached_sound(channel);
		actors.push_back(actor);
	}

	{
		const auto actor = game_engine_->get_renderer()->new_actor();
		actor->set_model_name("sphere");
		actor->set_color(glm::vec3(0.0f, 1.0f, 0.0f));
		actor->set_position(glm::vec3(0.0f,0.0f,-15.0f));
		actor->set_velocity(glm::vec3(0.0f, 0.0f, 2.5f));

		//we need to attach the sound to the object
		FMOD::Channel* channel;
		game_engine_->get_audio_manager()->play_sound("engine", glm::vec3(), 0.1f, &channel);
		actor->set_attached_sound(channel);
		actors.push_back(actor);
	}

	{
		const auto actor = game_engine_->get_renderer()->new_actor();
		actor->set_model_name("sphere");
		actor->set_color(glm::vec3(1.0f, 0.0f, 0.0f));
		actor->set_position(glm::vec3(15.0f, 0.0f, -15.0f));
		actor->set_velocity(glm::vec3(-1.0f, 0.0f, 0.5f));

		//we need to attach the sound to the object
		FMOD::Channel* channel;
		game_engine_->get_audio_manager()->play_sound("engine", glm::vec3(), 0.1f, &channel);
		actor->set_attached_sound(channel);
		actors.push_back(actor);
	}

	{
		const auto actor = game_engine_->get_renderer()->new_actor();
		actor->set_model_name("sphere");
		actor->set_color(glm::vec3(1.0f, 0.0f, 0.0f));
		actor->set_position(glm::vec3(15.0f, 0.0f, 0.0f));
		actor->set_velocity(glm::vec3(-1.0f, 0.0f, -0.5f));

		//we need to attach the sound to the object
		FMOD::Channel* channel;
		game_engine_->get_audio_manager()->play_sound("engine", glm::vec3(), 0.1f, &channel);
		actor->set_attached_sound(channel);
		actors.push_back(actor);
	}


	while (should_run())
	{
		const auto camera_pos = game_engine_->get_renderer()->get_camera_position();
		printf("x:%f y:%f z:%f\n", camera_pos.x, camera_pos.y, camera_pos.z);

		for (auto actor : actors)
		{
			const auto sound = actor->get_attached_sound();
			if(sound)
			{
				//do physics stuff here
				auto current_position = actor->get_position();
				auto current_velocity = actor->get_velocity();
				current_position += current_velocity * game_engine_->get_renderer()->get_frame_delta_time();
				actor->set_position(current_position);

				constexpr float cube_size = 20.0f;
				//bounds check to see if we are in our invisible cube
				if(fabs(current_position.x) > cube_size)
				{
					current_position.x = current_position.x > 0 ? cube_size : -cube_size;
					current_velocity.x *= -1.0f;

					FMOD::Channel* channel;
					game_engine_->get_audio_manager()->play_sound("impact", current_position, 0.25f, &channel);
				}

				if (fabs(current_position.y) > cube_size)
				{
					current_position.y = current_position.y > 0 ? cube_size : -cube_size;
					current_velocity.y *= -1.0f;

					FMOD::Channel* channel;
					game_engine_->get_audio_manager()->play_sound("impact", current_position, 0.25f, &channel);
				}

				if (fabs(current_position.z) > cube_size)
				{
					current_position.z = current_position.z > 0 ? cube_size : -cube_size;
					current_velocity.z *= -1.0f;

					FMOD::Channel* channel;
					game_engine_->get_audio_manager()->play_sound("impact", current_position, 0.25f, &channel);
				}
				actor->set_velocity(current_velocity);

				//check if collision with other spheres
				bool did_collide = false;
				for (IActor* collision_actor : actors)
				{
					if(collision_actor != actor)
					{
						if(glm::distance(collision_actor->get_position(), actor->get_position()) <= 1.0f)
						{
							did_collide = true;
							{
								auto direction = collision_actor->get_position() - actor->get_position();
								direction = glm::normalize(direction);
								auto new_vel = 5.0f * -direction;
								actor->set_velocity(new_vel);
							}
							{
								auto direction = actor->get_position() - collision_actor->get_position();
								direction = glm::normalize(direction);
								auto new_vel = 5.0f * -direction;
								actor->set_velocity(new_vel);
							}

							printf("collision\n");
						}
					}
				}

				if(did_collide)
				{
					FMOD::Channel* channel;
					game_engine_->get_audio_manager()->play_sound("metal_impact", actor->get_position(), 1.0f, &channel);
				}

				//update the sound position and velocity
				game_engine_->get_audio_manager()->update_3d_sound_position(actor->get_attached_sound(), current_position);
			}
		}

		game_engine_->get_renderer()->tick(actors);
		game_engine_->get_audio_manager()->tick(camera_pos);
	}

	for (const auto& actor : actors)
	{
		delete actor;
	}
	actors.clear();

	return 0;
}

void Application::shutdown()
{
	game_engine_->shutdown();
	delete game_engine_;
	game_engine_ = nullptr;
}
