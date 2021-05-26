#pragma once
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <iostream>
#include <cstdio>
#include "util.h"
#include "Ear.h"
#include "Positional.h"

class Sound : public Positional
{
public:
	Sound(const char* fname, bool loop = false, float local = 0.0, float loudnes = 1.0, unsigned setchannel = 0, unsigned setsamplerate = 0) 
		: looped(loop), local(local), loudness(loudnes)
	{
		load(fname, setchannel, setsamplerate);
		data = trackpool[fname].data.data();
		size = trackpool[fname].data.size();
		channel = trackpool[fname].channel;
		samplerate = trackpool[fname].samplerate;

	}
	Sound(const Sound& other)
		: looped(other.looped), size(other.size), pos(other.pos), last_sample(other.last_sample),
		  data(other.data), local(other.local), loudness(other.loudness), channel(other.channel), samplerate(other.samplerate)
	{

	}
	static void set_loudness(const char* trackname, double param_loudness)
	{
		if (trackpool.find(trackname) != trackpool.end())
		{
			double fac = 0.0;
			for (double datapoint : trackpool.at(trackname).data)
			{
				double res = datapoint / param_loudness;
				fac = res > fac ? res : fac;
			}
			double* dat = trackpool.at(trackname).data.data();
			size_t sz = trackpool.at(trackname).data.size();
			for (size_t i = 0; i < sz; i++)
			{
				dat[i] *= fac;
			}
		}
	}
private:
	friend class Audio;
	bool is_done() const { return !(looped) && (pos == size); }
	unsigned get_samplerate() const { return samplerate; }
	double read(unsigned c)
	{
		if (c < channel)
		{
			pos %= size;
			last_sample = data[pos++];
		}
		if (local && ear)
		{
			float fac_a = ear->inv_sqrd_distance_to(get_x(), get_y()) * local;
			fac_a = (fac_a > 1.0f ? 1.0f : fac_a) * loudness;
			return last_sample * fac_a;
		}
		return last_sample * loudness;
	}
private:
	static Ear* ear;
	size_t size = 0, pos = 0;
	double last_sample = 0.0;
	double* data = nullptr;
	bool looped = false;
	float local = 0.0, loudness = 1.0;
	uint16_t channel = 0;
	unsigned samplerate = 0;

	struct trackinfo {
		std::vector<double> data;
		uint16_t channel = 0;
		unsigned samplerate = 0;
	};

	static std::unordered_map<std::string, trackinfo> trackpool;
	void load(const char* fname, unsigned setchannel = 0, unsigned setsamplerate = 0, bool raw = false);

	template<typename T>
	double clamp(double in, T max)
	{
		return in / (double)max;
	}
	template<typename T>
	std::vector<double> resample(const std::vector<T>& in, unsigned to, unsigned channels, unsigned no_frames, bool mute = false)
	{
		double ratio = (double)to / (double)samplerate;
		std::vector<double> data((size_t)((double)no_frames * ratio) * channels);
		T max = (T)pow(2, (sizeof(T) * 8) - 1) - 1;
		double dmax = (double)max;
		size_t last = 0;
		if (ratio < 1.0)
		{
			for (size_t i = channel; i < in.size(); i += channel) {
				double weight = ratio * (double)(i / channel);
				size_t data_idx = (size_t)weight;
				weight = weight - (double)data_idx;
				for (size_t j = 0; j < channels; j++) {
					if (j >= channel) {
						data[data_idx * channels + j] = mute ? 0.0 : clamp<T>((double)in[i], max);
					}
					else {
						double val = weight * in[i + j] + (1 - weight) * in[last + j];
						data[data_idx * channels + j] = clamp<T>(val, max);
					}
				}
				last = i;
			}
		}
		else if (ratio > 1.0)
		{
			ratio = (double)samplerate / (double)to;
			for (size_t i = channels; i < data.size(); i += channels) {
				double weight = ratio * (double)(i / channels);
				size_t data_idx = (size_t)weight;
				weight = weight - (double)data_idx;
				data_idx *= channel;
				for (unsigned j = 0; j < channels; j++) {
					if (j >= channel) {
						data.at(i + j) = mute ? 0.0 : clamp<T>((double)in.at(data_idx), max);
					}
					else {
						double val = weight * in.at(data_idx + j) + (1 - weight) * in.at(last + j);
						data.at(i + j) = clamp<T>(val, max);
					}
				}
				last = data_idx;
			}
		}
		else
		{
			for (size_t i = 0, k = 0; i < in.size(); i += channel, k += channels) {
				for (size_t j = 0; j < channels; j++) {
					if (j >= channel) {
						data[k + j] = mute ? 0.0 : clamp<T>((double)in[i], max);
					}
					else {
						data[k + j] = clamp<T>((double)in[i + j], max);
					}
				}
			}
		}
		return data;
	}
};

