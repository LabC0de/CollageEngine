#include "Sound.h"
#include "Audio.h"
#include "Debug.h"

std::unordered_map<std::string, Sound::trackinfo> Sound::trackpool;
Ear* Sound::ear = nullptr;
Audio* Audio::active_obj = nullptr;

void Sound::load(const char* fname, unsigned setchannel, unsigned setsamplerate, bool raw)
{
	if (trackpool.find(fname) != trackpool.end()) { return; }

	std::ifstream infile(fname, std::ios::in | std::ios::binary);
	if (!infile.good())
		throw - 1;

	char identifiers[5] = { 0 };
	infile.read(identifiers, 4);
	if (strcmp(identifiers, "RIFF"))
		throw - 1;
	DBG(printf("Processing: %s ", identifiers);)


		unsigned fsize;
	infile.read((char*)&fsize, 4);				if (is_big_endian()) { fsize = swap32(fsize); }
	fsize += 8;

	infile.read(identifiers, 4);
	if (strcmp(identifiers, "WAVE"))
		throw - 1;
	DBG(printf("%s file named %s sized %d\n", identifiers, fname, fsize));

	infile.read(identifiers, 4);
	if (strcmp(identifiers, "fmt "))
		throw - 1;
	DBG(std::cout << "Processing FMT header:\n");

	unsigned chuncksize;
	infile.read((char*)&chuncksize, 4);			if (is_big_endian()) { chuncksize = swap32(chuncksize); }
	if (chuncksize != 16)
		throw - 1;

	uint16_t mode;
	infile.read((char*)&mode, 2);				if (is_big_endian()) { mode = swap16(mode); }
	switch (mode)
	{
	case 1:
		break; // only pcm supported
	default:
		DBG(std::cout << "\tData format: NOT SUPPORTED\n");
		throw - 1;
	}
	DBG(std::cout << "\tData format: PCM (Uncompressed)\n");

	infile.read((char*)&channel, 2);			if (is_big_endian()) { channel = swap16(channel); }
	infile.read((char*)&samplerate, 4);			if (is_big_endian()) { samplerate = swap32(samplerate); }
	if (setchannel == 0) { setchannel = channel; }
	if (setsamplerate == 0) { setsamplerate = samplerate; }
	DBG(std::cout << "\tChannels: " << channel << " set to " << setchannel << " Samplerate: " << samplerate << " set to " << setsamplerate << "\n");

	unsigned bytes_per_second;
	infile.read((char*)&bytes_per_second, 4);	if (is_big_endian()) { bytes_per_second = swap32(bytes_per_second); }
	uint16_t frame_size, bits_per_sample;
	infile.read((char*)&frame_size, 2);			if (is_big_endian()) { frame_size = swap16(frame_size); }
	infile.read((char*)&bits_per_sample, 2);	if (is_big_endian()) { bits_per_sample = swap16(bits_per_sample); }
	DBG(std::cout << "\tSampleinfo: " << frame_size / channel << " bytes per channel with " << bits_per_sample << " bit audio.\n");

	infile.read(identifiers, 4);
	if (strcmp(identifiers, "data"))
		throw - 1;
	unsigned datasize;
	infile.read((char*)&datasize, 4);			if (is_big_endian()) { datasize = swap32(datasize); }
	DBG(std::cout << "Processing DATA header: (size: " << datasize << ")\n");

	std::vector<double> data;
	if (bits_per_sample <= 8)
	{
		std::vector<int8_t> tmp_data(datasize);
		tmp_data.assign(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
		data = resample(tmp_data, setsamplerate, setchannel, datasize / frame_size);

	}
	else if (bits_per_sample <= 16)
	{
		std::vector<int16_t> tmp_data(datasize / 2);
		infile.read((char*)&tmp_data[0], tmp_data.size() * 2);
		if (is_big_endian()) {
			for (unsigned i = 0; i < tmp_data.size(); i++) {
				tmp_data[i] = swap16(tmp_data[i]);
			}
		}
		data = resample(tmp_data, setsamplerate, setchannel, datasize / frame_size);
	}
	else if (bits_per_sample <= 32)
	{
		std::vector<int32_t> tmp_data(datasize / 4);
		infile.read((char*)&tmp_data[0], datasize);
		if (is_big_endian()) {
			for (unsigned i = 0; i < tmp_data.size(); i++) {
				tmp_data[i] = swap32(tmp_data[i]);
			}
		}
		data = resample(tmp_data, setsamplerate, setchannel, datasize / frame_size);
	}
	else
	{
		std::vector<int64_t> tmp_data(datasize / 8);
		infile.read((char*)&tmp_data[0], datasize);
		if (is_big_endian()) {
			for (unsigned i = 0; i < tmp_data.size(); i++) {
				tmp_data[i] = swap64(tmp_data[i]);
			}
		}
		data = resample(tmp_data, setsamplerate, setchannel, datasize / frame_size);
	}
	channel = (uint16_t)setchannel;
	samplerate = setsamplerate;
	trackpool[fname] = { data, channel, samplerate };
}