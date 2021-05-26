#pragma once
#include <unordered_map>
#include <string>
#include "Collected.h"


class Texture : public Collected
{
public:
	Texture(const char* fname);
	Texture(const Texture& other) : Collected(other), texture(other.texture), ratio(other.ratio) {}
	Texture& operator=(const Texture& other)
	{
		Collected::operator=(other);
		texture = other.texture;
		ratio = other.ratio;
		return *this;
	}
	inline bool operator==(const Texture& other) const { return texture == other.texture; }
	void use(int slot) const;
	inline float get_ratio() const { return ratio; }
	inline unsigned get_id() const { return texture; }
	static Texture pool(const char* fname)
	{
		auto result = map.find(fname);
		if (result == map.end())
		{
			map.emplace(fname, Texture(fname));
		}
		return map.at(fname);
	}
	static void flush_pool()
	{
		map.clear();
	}

	~Texture()
	{
		dtor();
	}
private:
	void clear() override;

	unsigned texture = 0;
	float ratio = 0.0f;
	static std::unordered_map<std::string, Texture> map;
	static unsigned ctr;
};