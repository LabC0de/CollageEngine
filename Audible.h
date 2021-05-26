#pragma once
#include "Audio.h"
#include "Object.h"


// Vector Safe
struct TrackDescriptor {
	const char* fname;
	bool loop = false;
	float loudnes = 1.0, local = 0.0;
	unsigned setchannel = DEFAULT_AUDIO_CHANNELS, setsamplerate = DEFAULT_AUDIO_SAMPLERATE;
};

class Audible : virtual public Object
{
public:
	Audible(std::initializer_list<const char*> fnames)
	{
		register_as(AUDIBLE);
		for (auto fname : fnames)
		{
			sounds.emplace_back(fname);
		}
	}
	Audible(std::initializer_list<TrackDescriptor> decrs)
	{
		register_as(AUDIBLE);
		for (const auto& dec : decrs)
		{
			sounds.emplace_back(dec.fname, dec.loop, dec.local, dec.loudnes, dec.setchannel, dec.setsamplerate);
		}
	}
	void play(unsigned i) const
	{
		Audio::sound_device()->play_sound(sounds[i]);
	}
protected:
	Audible& operator=(const Audible& other) {
		if (this != &other) sounds = other.sounds;
		return *this;
	}
	Audible& operator=(Audible&& other) noexcept {
		if (this != &other) sounds = std::move(other.sounds);
		return *this;
	}
	Audible(const Audible& other) : sounds(other.sounds) {}
	Audible(Audible&& other) noexcept : sounds(std::move(other.sounds)) {}
private:
	std::vector<Sound> sounds;
};