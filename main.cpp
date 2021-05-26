#include "Engine.h"
#include "Triggerbox.h"
#include "Physical.h"

/** NOTES
* <PHYSICS>
* Triggerbox:
*	- Fine grained collision detection (if object is inside triggerbox and doesnt cross line gegments) SAT?
* Big Constant Objects:
*	- Have to be a own class
*	- Border outside or border inside
*	- Line segments
*	- hitbox crosses line
*/

class Test : public Drawable, public Triggerbox, public Controlled, public Audible, public Timed
{
public:
	Test(float x, float y) :
		Positional(x, y), Spacial(50.0f), 
		Directional(0.0f), Triggerbox(x, y, 50.0f, 0.0f), 
		Drawable{ "rescources/green.png", "rescources/yellow.png" , "rescources/red.png" },
		Controlled{ {"Move Up", GLFW_KEY_W}, {"Move Left", GLFW_KEY_A},
					{"Move Down", GLFW_KEY_S}, {"Move Right", GLFW_KEY_D},
					{"Rotate Counterclockwise", GLFW_KEY_Q} , {"Rotate Clockwise", GLFW_KEY_E},
					{"Speed Up", GLFW_KEY_LEFT_SHIFT },
			},
		Audible{	{"rescources/ping.wav", false}	}
	{}
protected:
	void on_physics_test() override
	{
		change_active_texture(0);
	}
	void on_aabb_collision(Physical& other) override
	{
		change_active_texture(1);
	}
	void on_initial_contact(Physical* other) override
	{
		change_active_texture(2);
		play(0);
	}
	void on_continued_contact(Physical* other) override
	{
		change_active_texture(2);
	}
	void on_contact_end(Physical* other) override
	{
		change_active_texture(1);
	}
	void on_probe(Object* optional = nullptr) 
	{ 
		play(0);
	}
	void on_key(int key, int scancode, int action, int mod) override
	{
		bool pressed = false;
		switch (action)
		{
		case GLFW_PRESS:
			pressed = true;
		case GLFW_RELEASE:
			break;
		default:
			return;
		}
		switch (key)
		{
		case GLFW_KEY_W:
			vy = pressed ? 10.0f : 0.0f;
			break;
		case GLFW_KEY_A:
			vx = pressed ? -10.0f : 0.0f;
			break;
		case GLFW_KEY_S:
			vy = pressed ? -10.0f : 0.0f;
			break;
		case GLFW_KEY_D:
			vx = pressed ? 10.0f : 0.0f;
			break;
		case GLFW_KEY_Q:
			vang = pressed ? 10.0f : 0.0f;
			break;
		case GLFW_KEY_E:
			vang = pressed ? -10.0f : 0.0f;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			vx = pressed ? vx * 2.0f : vx / 2.0f;
			vy = pressed ? vy * 2.0f : vy / 2.0f;
			vang = pressed ? vang * 2.0f : vang / 2.0f;
		}
	}
	void timed_update(float dt) override
	{
		if (vx != 0.0f || vy != 0.0f)
		{
			move(vx * dt, vy * dt);
		}
		if (vang != 0.0f)
		{
			rotate(vang * dt);
		}
	}
	void set_rotation(float deg) override
	{
		Directional::set_rotation(deg);
		Drawable::update_batch();
		Physical::invalidate();
		Physical::update_aabb();
	}
	void set_size(float sz) override
	{
		Spacial::set_size(sz);
		Drawable::update_batch();
		Physical::invalidate();
		Physical::update_aabb();
	}
	void set_x(float nx) override
	{
		Positional::set_x(nx);
		Drawable::update_batch();
		Physical::invalidate();
		Physical::update_aabb();
	}
	void set_y(float ny) override
	{
		Positional::set_y(ny);
		Drawable::update_batch();
		Physical::invalidate();
		Physical::update_aabb();
	}
private:
	float vx = 0.0, vy = 0.0, vang = 0.0;
};

class TestGame : public Engine, public Controlled
{
public:
	TestGame() 
		: Engine(800, 600), Controlled(USE_MOUSE_MOVEMENT | USE_MOUSE_BUTTONS, { {"End Game", GLFW_KEY_ESCAPE} }),
			obj1{ "rescources/clckhnd.png" }, obj2{ "rescources/clckhnd.png" },
			t1(-100, 0), t2(100, 0), cam(800.0, 600.0)
	{
		obj1.scale(100.0);
		obj2.scale(50.0);
		layer.add(obj1);
		layer.add(obj2);
		layer.add(t1);
		layer.add(t2);
		handle(layer);
		handle(*this);
		handle(t1);
		handle(t2, NO_TIMED_UPDATE | NO_INPUT);
		this->main();
	}
private:
	void main() override
	{
		start();
		while (!done()) {
			float dt = tick();
			obj1.rotate(7 * -dt);
			obj2.rotate(-dt);
		}
	}
	void on_key(int key, int scancode, int action, int mod) override
	{
		kill();
	}
	void on_mouse_move(double xpos, double ypos) override 
	{ 
		mouse_x = (float)xpos;
		mouse_y = (float)ypos;
	}
	void on_mouse_click(int button, int action, int mods) override 
	{ 
		if(action == GLFW_PRESS) probe(mouse_x, mouse_y);
	}
	Camera cam;
	Drawable obj1, obj2;
	Test t1, t2;
	DynamicLayer layer;
	float mouse_x = 0.0, mouse_y = 0.0;
};


int main()
{
	TestGame tst;
	printf("AVG frametime %f -> FPS: %f\n", tst.frametime(), 1/tst.frametime());
	return 0;
}