#pragma once

class Engine;

class Level
{
public:
	virtual void on_level_load(Engine* eng) {
		return;
	}
	virtual void on_tick(Engine* eng) {
		return;
	}
};
