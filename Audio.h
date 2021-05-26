#pragma once

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <list>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include "Sound.h"
#include "Defines.h"

class Audio
{
public:
	Audio(const std::wstring& dev, 
		unsigned samplerate = DEFAULT_AUDIO_SAMPLERATE, unsigned channels = DEFAULT_AUDIO_CHANNELS,
		unsigned n_blocks = 8, unsigned samples_block = 512)
		: msamplerate(samplerate), n_blocks_free(no_blocks), nchannels(channels), samples_in_block(samples_block), no_blocks(n_blocks)
	{
		if (Audio::is_initialized()) return;

		int ndevices = waveOutGetNumDevs();
		std::vector<std::wstring> devs;
		WAVEOUTCAPS woc;
		int dev_id = 0;
		for (int n = 0; n < ndevices; n++) {
			if (waveOutGetDevCaps(n, &woc, sizeof(WAVEOUTCAPS)) == S_OK) {
				if (dev == woc.szPname) {
					dev_id = n;
				}
				devs.push_back(woc.szPname);
			}
		}
		if (ndevices != 0)
		{
			WAVEFORMATEX waveFormat;
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nSamplesPerSec = samplerate;
			waveFormat.wBitsPerSample = sizeof(short) * 8;
			waveFormat.nChannels = channels;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
			waveFormat.cbSize = 0;

			// Open Device if valid
			if (waveOutOpen(&hwdev, dev_id, &waveFormat, (DWORD_PTR)waveOutProcWrap, (DWORD_PTR)nullptr, CALLBACK_FUNCTION) != S_OK) {
				throw - 1;
			}
			if (active_obj != nullptr)
				throw - 1;
			active_obj = this;
		}
		data.assign(samples_block * n_blocks, 0);
		blocks.resize(n_blocks);
		for (unsigned i = 0; i < n_blocks; i++) {
			blocks[i].dwBufferLength = sizeof(short) * samples_block;
			blocks[i].lpData = (LPSTR)(&data[0] + (i * samples_block));
		}
		running = true;
		audio_thread = std::thread(&Audio::audio_main, this);

		std::unique_lock<std::mutex> lm(wait_for_blocks);
		m_cv_wait_for_blocks.notify_one();
	}
	static Audio* sound_device() { return active_obj; }
	static bool is_initialized() { return active_obj != nullptr; }

	void play_sound(const Sound& sound)
	{
		std::unique_lock<std::mutex> lock(wait_for_blocks);
		sounds.emplace_back(sound);
	}
	static std::vector<std::wstring> devices()
	{
		int ndevices = waveOutGetNumDevs();
		std::vector<std::wstring> ret;
		WAVEOUTCAPS woc;
		int dev_id = 0;
		for (int n = 0; n < ndevices; n++) {
			if (waveOutGetDevCaps(n, &woc, sizeof(WAVEOUTCAPS)) == S_OK) {
				ret.push_back(woc.szPname);
			}
		}
		return ret;
	}
	static void place_ear(Ear* ear)
	{
		Sound::ear = ear;
	}
	void kill()
	{
		running = false;
		audio_thread.join();
		waveOutClose(hwdev);
	}
	void reset()
	{
		std::unique_lock<std::mutex> lock(wait_for_blocks);
		sounds.clear();
	}
private:
	HWAVEOUT hwdev;
	unsigned msamplerate;
	unsigned nchannels;
	unsigned samples_in_block;
	unsigned no_blocks;
	unsigned current_block = 0;

	std::vector<short> data;
	std::vector<WAVEHDR> blocks;
	std::list<Sound> sounds;

	std::thread audio_thread;
	std::atomic<bool> running;
	std::atomic<unsigned int> n_blocks_free;
	std::condition_variable m_cv_wait_for_blocks;
	std::mutex wait_for_blocks;

	static Audio* active_obj;

	void waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2)
	{
		if (uMsg != WOM_DONE) return;
		n_blocks_free.fetch_add(1);
		std::unique_lock<std::mutex> lm(wait_for_blocks);
		m_cv_wait_for_blocks.notify_one();
	}
	// Static wrapper for sound card handler
	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		active_obj->waveOutProc(hWaveOut, uMsg, dwParam1, dwParam2);
	}
	double clip(double sample)
	{
		if (sample > 1.0) { return  1.0; }
		if (sample < -1.0) { return -1.0; }
		return sample;
	}
	short to_16bits(double cliped_sample)
	{
		return (short)(cliped_sample * 32767.0);
	}
	double source(unsigned channel)
	{
		double res = 0.0;
		for (auto it = sounds.begin(); it != sounds.end();)
		{
			if (it->is_done()) {
				it = sounds.erase(it);
			}
			else {
				res += it->read(channel);
				++it;
			}
		}
		return res;
	}
	void audio_main()
	{
		short nprev_sample = 0;
		while (running)
		{
			if (n_blocks_free == 0)
			{
				std::unique_lock<std::mutex> lm(wait_for_blocks);
				while (n_blocks_free == 0) m_cv_wait_for_blocks.wait(lm);
			}
			n_blocks_free--;
			if (blocks[current_block].dwFlags & WHDR_PREPARED) {
				waveOutUnprepareHeader(hwdev, &blocks[current_block], sizeof(WAVEHDR));
			}
			short newsample = 0;
			unsigned offset = current_block * samples_in_block;
			for (unsigned n = 0; n < samples_in_block; n += nchannels)
			{
				for (unsigned c = 0; c < nchannels; c++)
				{
					data[offset + n + c] = to_16bits(clip(source(c)));
				}
			}
			waveOutPrepareHeader(hwdev, &blocks[current_block], sizeof(WAVEHDR));
			waveOutWrite(hwdev, &blocks[current_block], sizeof(WAVEHDR));
			current_block = (current_block + 1) % no_blocks;
		}
	}
};