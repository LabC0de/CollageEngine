#pragma once
#include <type_traits>
#include "Renderer.h"
#include "Input.h"
#include "Timer.h"
#include "Audible.h"
#include "Physics.h"
#include "Level.h"
#include "Camera.h"

enum {
	NO_RENDER = 0x01,
	NO_COLLISION = 0x02,
	NO_INPUT = 0x04,
	NO_TIMED_UPDATE = 0x08
};

class Engine
{
public:
	template <typename T>
	void handle(T& obj, uint64_t bitmask = 0) {
		if constexpr (std::is_same<DynamicLayer, T>::value) {
			collage.add_layer(obj);
		}
		if constexpr (std::is_base_of<Physical, T>::value) {
			if (!(bitmask & 0x02)) collision.make_entry(obj);
		}
		if constexpr (std::is_base_of<Controlled, T>::value) {
			if(!(bitmask & 0x04)) collector.make_entry(obj);
		}
		if constexpr (std::is_base_of<Timed, T>::value) {
			if (!(bitmask & 0x08)) Timer::make_entry(obj);
		}
	}

	template <typename T>
	void remove(T& obj, uint64_t bitmask = 0) {
		if constexpr (std::is_base_of<Drawable, T>::value) {
			if (!(bitmask & 0x01)) obj.lay_ptr()->remove(obj);
		}
		if constexpr (std::is_base_of<Physical, T>::value) {
			if (!(bitmask & 0x02)) collision.remove_entry(obj);
		}
		if constexpr (std::is_base_of<Controlled, T>::value) {
			if (!(bitmask & 0x04)) collector.remove_entry(obj);
		}
		if constexpr (std::is_base_of<Timed, T>::value) {
			if (!(bitmask & 0x08)) Timer::remove_entry(obj);
		}
	}

	Physical* probe(float x, float y, Object* optional = nullptr)
	{
		x -= (float)width / 2;
		y = (float)height / 2 - y;
		return collision.probe(x, y, optional);
	}

	void play_sound(const Sound& snd) {
		cool.play_sound(snd);
	}

	void load_level(Level* lv) {
		if (active_level) reset();
		lv->on_level_load(this);
		active_level = lv;
	}
	void kill() {
		b_kill = true;
	}

	void reset() {
		collector.reset();
		cool.reset();
		collision.reset();
		collage.reset();
		Timer::reset();
	}

	virtual void main() 
	{
		start();
		while (!done()) {
			//active_level->on_tick(this);
			tick();
		}
	}

	double frametime() const {
		return uptime / (double)framecounter;
	}
public:
	Engine(int widh, int height, const char* title = "default");
	bool done();
	virtual ~Engine();
	float clock()
	{
		return Timer::clock();
	}
	void start()
	{
		Timer::init();
	}
	float tick()
	{
		collision.calculate();
		collage.clear_color();
		collage.draw();
		collage.swap_buffer(); // includes input polling -> has to go into the correct class later
		float ret = clock();
		uptime += ret;
		++framecounter;
		return ret;
	}
private:
	bool b_kill = false;
	GLFWwindow* window = nullptr;
	Renderer collage;
	Input collector;
	Audio cool;
	Physics collision;
	Level* active_level = nullptr;

	double uptime = 0.0;
	size_t framecounter = 0;
	int width, height;
	static void error_callback(int error, const char* description)
	{
		printf("GLFW Error: %s\n", description);
	}
};

