#pragma once
#include "Texture.h"

struct SPSTexCoords {
	float x = 0, y = 0, dx = 1, dy = 1;
};

class Spritesheet
{
public:
	Spritesheet(const char* fname, const std::vector<SPSTexCoords>& texmap = { {0,0,1,1} }) : sheet(Texture::pool(fname))
	{
		sps_map[sheet.get_id()] = texmap;
	}
	SPSTexCoords get_active_texture() const {
		return sps_map.at(sheet.get_id()).at(active_texture);
	}
	void set_active_texture(unsigned idx) {
		active_texture = idx;
	}
	void use(int slot) const
	{
		sheet.use(slot);
	}
	bool operator==(const Spritesheet& other) { return sheet == other.sheet; }
	float get_ratio() const 
	{ 
		auto tmp = sps_map.at(sheet.get_id()).at(active_texture);
		return tmp.dx / tmp.dy;
	}
private:
	unsigned active_texture = 0;
	Texture sheet;
	static std::unordered_map<unsigned, std::vector<SPSTexCoords>> sps_map;
};